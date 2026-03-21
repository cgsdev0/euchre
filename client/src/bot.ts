import type { ServerMsg, WelcomeMsg } from "types/api.ts";
import { WebSocket } from "ws";

const room = process.argv[2];
const bots = Number.parseInt(process.argv[3]);

function pick_random<T>(arr: Array<T>): T {
  return arr[Math.floor(Math.random() * arr.length)];
}

let suits = ["hearts", "diamonds", "clubs", "spades"];
interface BotState extends WelcomeMsg {}
type State = Omit<BotState, "type">;
const initial_state: State = {
  id: 0,
  played_cards: [],
  trump: "hearts",
  your_cards: [],
  trick: [],
  scores: [0, 0],
  players: [],
  rich_chat_log: [],
  turn: 0,
  dealer: 0,
  phase: "lobby",
  private_session: false,
};
const startBot = (room: string, i: number) => {
  function log(...args: any[]) {
    if (state.id !== 0) return;
    console.log(...args);
  }
  let cookie = `bot${i}`;
  let state: State = JSON.parse(JSON.stringify(initial_state)) as State;
  const setupWs = () => {
    const ws = new WebSocket(`ws://localhost:3001/ws/room/${room}`, {
      headers: { Cookie: `_session=${cookie}` },
    });

    ws.addEventListener("open", () => {
      log("opened", room);
      send("update_name", { name: cookie });
    });

    ws.addEventListener("error", () => {
      log("error!");
    });
    return ws;
  };

  let ws = setupWs();

  const send = (type: string, msg?: object) => {
    ws.send(JSON.stringify({ ...{ type }, ...(msg || {}) }));
  };
  const perform = () => {
    if (state.turn !== state.id) {
      return;
    }
    switch (state.phase) {
      case "vote_round1":
        send("pass");
        break;
      case "discarding":
        {
          const card = state.your_cards.shift();
          send("discard", { card });
        }
        break;
      case "playing":
        if (i === 0) {
          while (state.your_cards.length) {
            const card = state.your_cards.shift();
            send("premove", { card });
          }
        } else {
          setTimeout(() => {
            const card = state.your_cards.shift();
            send("play_card", { card });
          }, 1000);
        }
        break;
      case "vote_round2":
        if (state.turn !== state.dealer) {
          send("pass");
        } else {
          send("order", {
            suit: pick_random(suits.filter((s) => s !== state.top_card.suit)),
          });
        }
        break;
    }
  };
  const messageHandler = (e: WebSocket.MessageEvent) => {
    try {
      const data = JSON.parse(e.data.toString()) as string | ServerMsg;
      if (typeof data === "string") {
        console.error(data);
        return;
      }
      log("[message]", data.type);
      switch (data.type) {
        // @ts-ignore
        case "error":
          console.error(data);
          break;
        case "deal":
          state = { ...state, ...data };
          log(data);
          break;
        case "order":
          if (state.phase === "vote_round1") {
            if (state.id === state.dealer) {
              state.your_cards.push(state.top_card);
            }
          }
          break;
        case "update":
          state = { ...state, ...data };
          perform();
          break;
        case "welcome":
          state = { ...state, ...data };
          if (state.phase === "lobby" && state.players.length === 4) {
            ws.send(JSON.stringify({ type: "restart" }));
          }
          log(state);
          perform();
          break;
        // @ts-ignore
        case "redirect":
          // @ts-ignore
          room = data.room;
          ws.close(4242);
          ws = setupWs();
          ws.addEventListener("message", messageHandler);
          ws.addEventListener("close", closeHandler);
          break;
      }
    } catch (e: any) {
      console.error(e);
    }
  };
  const closeHandler = (e: WebSocket.CloseEvent) => {
    if (e.code === 4242) return;
    log("closed, reopening...");
    setTimeout(() => {
      ws = setupWs();
      ws.addEventListener("message", messageHandler);
      ws.addEventListener("close", closeHandler);
    }, 1000);
  };
  ws.addEventListener("close", closeHandler);

  ws.addEventListener("message", messageHandler);
};

for (let i = 0; i < bots; ++i) {
  startBot(room, i);
}
