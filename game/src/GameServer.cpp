#include <cstdlib>
#include <iostream>
#include <json.hpp>
#include <random>
#include <set>

#include <regex>
#include <signal.h>
#include <string>
#include <string_view>
#include <unistd.h>
#include <variant>

#include "App.h"
#include "Consts.h"
#include "Game.h"

#include "GameCoordinator.h"
#include "MoveOnlyFunction.h"

#define DONT_REUSE_THE_FUCKING_PORT_LINUX 1

// for convenience
using json = nlohmann::json;

typedef uWS::HttpResponse<false> HttpResponse;
typedef uWS::HttpRequest HttpRequest;

std::function<void(void)> signal_handler;
void signal_callback_handler(int signum) {
    signal_handler();
}

std::string getSession(HttpRequest *req) {
    const std::regex sessionRegex("_session=([^;]*)");
    std::smatch cookies;
    std::string s(req->getHeader("cookie"));
    std::regex_search(s, cookies, sessionRegex);
    return "guest:" + std::string(cookies[1].str());
}

void connectNewPlayer(uWS::App *app, uWS::WebSocket<false, true, PerSocketData> *ws, PerSocketData *userData, GameCoordinator &coordinator) {
    auto it = coordinator.games.find(userData->room);
    if (it != coordinator.games.end()) {
        // Connecting to a valid game
        Game *g = it->second;

        if (!g->hasPlayer(userData->session)) {
            try {
                auto msg = g->addPlayer(*userData);
                app->publish(userData->room, msg.toString(), uWS::OpCode::TEXT);
            } catch (const API::GameError &error) {
                ws->send(error.toString(), uWS::OpCode::TEXT);
                ws->close();
                return;
            }
        } else {
            auto resp = g->reconnectPlayer(userData->session, *userData);
            if (resp) {
                app->publish(userData->room, resp->toString(), uWS::OpCode::TEXT);
            }
        }
        auto msg = g->toWelcomeMsg(userData->session);
        ws->send(msg.toString(), uWS::OpCode::TEXT);
        ws->subscribe(userData->room);
    } else {
        // Connecting to a non-existant room
        // let's migrate them to a new room
        userData->room = coordinator.createRoom(true, userData->room);
        API::Redirect msg{.room = userData->room};
        ws->send(msg.toString(), uWS::OpCode::TEXT);
    }
}

uWS::App::WebSocketBehavior<PerSocketData> makeWebsocketBehavior(uWS::App *app, GameCoordinator &coordinator) {

    return {/* Settings */
            .compression = uWS::SHARED_COMPRESSOR,
            .maxPayloadLength = 16 * 1024,
            /* Handlers */
            .upgrade =
                [&, app](auto *res, auto *req, auto *context) {
                    std::string session = getSession(req);
                    std::string room = std::string(req->getParameter(0));
                    bool dedupe_conns = false;
                    auto it = coordinator.games.find(room);
                    if (it != coordinator.games.end()) {
                        // Connecting to a valid game
                        Game *g = it->second;
                        if (g->isPlayerConnected(session)) {
                            dedupe_conns = true;
                        }
                    }
                    res->template upgrade<PerSocketData>(
                        {.session = session,
                         .room = room,
                         .display_name = "",
                         .dedupe_conns = dedupe_conns},
                        req->getHeader("sec-websocket-key"),
                        req->getHeader("sec-websocket-protocol"),
                        req->getHeader("sec-websocket-extensions"),
                        context);
                },
            .open =
                [&, app](auto *ws) {
                    PerSocketData *userData =
                        (PerSocketData *)ws->getUserData();

                    if (userData->session == "guest:") {
                        ws->send("cookie", uWS::OpCode::TEXT);
                        return;
                    }
                    if (userData->dedupe_conns) {
                        json resp;
                        resp["error"] = "already connected";
                        ws->send(resp.dump(), uWS::OpCode::TEXT);
                        return;
                    }
                    connectNewPlayer(app, ws, userData, coordinator);
                },
            .message =
                [&, app](auto *ws, std::string_view message, uWS::OpCode opCode) {
                    PerSocketData *userData =
                        (PerSocketData *)ws->getUserData();

                    if (userData->dedupe_conns) return;

                    std::string room = userData->room;
                    std::string session = userData->session;
                    std::string response;
                    try {
                        auto it = coordinator.games.find(room);
                        if (it != coordinator.games.end()) {
                            Game *g = it->second;
                            g->handleMessage(
                                {
                                    .broadcast = [ws, room, app](auto s) { app->publish(room, s, uWS::OpCode::TEXT); },
                                    .send =
                                        [ws](auto s) { ws->send(s, uWS::OpCode::TEXT); },
                                    .session = session,
                                },
                                message);
                        } else {
                            response = API::GameError({.error = "Room not found: " + room}).toString();
                        }
                    } catch (API::GameError &e) {
                        response = e.toString();
                    } catch (nlohmann::detail::parse_error &e) {
                        std::cout << "RECEIVED BAD JSON (parse_error): " << message
                                  << "\n"
                                  << e.what() << std::endl;
                        response = API::GameError({.error = e.what()}).toString();
                    } catch (nlohmann::detail::type_error &e) {
                        std::cout << "HANDLED BAD JSON (type_error): " << message
                                  << "\n"
                                  << e.what() << std::endl;
                        response = API::GameError({.error = e.what()}).toString();
                    } catch (nlohmann::detail::invalid_iterator &e) {
                        std::cout << "HANDLED BAD JSON (invalid_iterator): " << message
                                  << "\n"
                                  << e.what() << std::endl;
                        response = API::GameError({.error = e.what()}).toString();
                    } catch (nlohmann::detail::out_of_range &e) {
                        std::cout << "HANDLED BAD JSON (out_of_range): " << message
                                  << "\n"
                                  << e.what() << std::endl;
                        response = API::GameError({.error = e.what()}).toString();
                    } catch (nlohmann::detail::other_error &e) {
                        std::cout << "HANDLED BAD JSON (other_error): " << message
                                  << "\n"
                                  << e.what() << std::endl;
                        response = API::GameError({.error = e.what()}).toString();
                    }
                    if (response.length())
                        ws->send(response, uWS::OpCode::TEXT);
                },
            .close =
                [&, app](auto *ws, int code, std::string_view message) {
                    PerSocketData *userData =
                        (PerSocketData *)ws->getUserData();

                    if (userData->dedupe_conns) return;

                    std::string room = userData->room;
                    std::string session = userData->session;
                    auto it = coordinator.games.find(room);
                    if (it != coordinator.games.end()) {
                        Game *g = it->second;
                        auto resp = g->disconnectPlayer(session);
                        if (resp) {
                            app->publish(room, resp->toString(), uWS::OpCode::TEXT);
                        }
                        if (!g->connectedPlayerCount()) {
                            coordinator.queue_eviction(room);
                        }
                    }
                }};
}

int main(int argc, char **argv) {

    GameCoordinator coordinator;

    signal_handler = [&]() {
        std::cout << "Caught signal, attempting graceful shutdown..." << std::endl;
        coordinator.save_to_disk();
        exit(0);
    };

    signal(SIGINT, signal_callback_handler);
    signal(SIGTERM, signal_callback_handler);

    int port = DEFAULT_PORT;
    if (argc == 2) {
        // try to read a port as arg 2
        port = atoi(argv[1]);
    }
    try {
        coordinator.load_persistence();
    } catch (const std::exception &e) {
        std::cout << "WARNING: Failed to load server state from persistence!" << std::endl;
        std::cout << e.what() << std::endl;
    }

    uWS::App app;
    app.get("/create", [&](auto *res, auto *req) {
           std::string session = getSession(req);
           bool isPrivate = true;
           if (req->getQuery().find("public") != std::string::npos) {
               isPrivate = false;
           }
           if (session == "") {
               res->end();
           } else {
               res->end(coordinator.createRoom(isPrivate));
           }
       })
        .ws<PerSocketData>("/ws/:room", makeWebsocketBehavior(&app, coordinator))
        // This 1 makes it so that the port is not reusable
        // because it SUCKS ASS when the port is reusable and you
        // don't realize the port is reusable and then you have a bad day
        // because an old zombie process of the server is running somewhere
        // on your machine and you don't reboot very frequently and people told
        // you to try rebooting but you didnt because that's just a meme people
        // say, they don't actually mean it (usually), but in this case it would
        // have been a good idea, and anyways yeah just make it 1 instead of 0
        // and you should be fine ok bye
        //
        // p.s. chatpgt says:
        // Remember to reboot periodically as advised, as it can mitigate such issues.
        // Make the adjustment and proceed confidently.
        .listen(port, DONT_REUSE_THE_FUCKING_PORT_LINUX, [port](auto *socket) {
            if (socket) {
                std::cout << "Listening on port " << port << std::endl;
            }
        })
        .run();

    std::cout << "Failed to listen on port " << port << std::endl;
}
