#include <optional>
#include "json.hpp"
#include "helper.hpp"
#include "API.hpp"




namespace API {
void from_json(const json & j, Card & x);
void to_json(json & j, const Card & x);

void from_json(const json & j, ClientMsg & x);
void to_json(json & j, const ClientMsg & x);

void from_json(const json & j, CardElement & x);
void to_json(json & j, const CardElement & x);

void from_json(const json & j, RichTextChunk & x);
void to_json(json & j, const RichTextChunk & x);

void from_json(const json & j, TaggedCard & x);
void to_json(json & j, const TaggedCard & x);

void from_json(const json & j, Player & x);
void to_json(json & j, const Player & x);

void from_json(const json & j, RichTextMsg & x);
void to_json(json & j, const RichTextMsg & x);

void from_json(const json & j, ServerMsg & x);
void to_json(json & j, const ServerMsg & x);

void from_json(const json & j, PassMsg & x);
void to_json(json & j, const PassMsg & x);

void from_json(const json & j, PlayJajaDingDongMsg & x);
void to_json(json & j, const PlayJajaDingDongMsg & x);

void from_json(const json & j, TableTalkMsg & x);
void to_json(json & j, const TableTalkMsg & x);

void from_json(const json & j, OrderMsg & x);
void to_json(json & j, const OrderMsg & x);

void from_json(const json & j, LastCardMsg & x);
void to_json(json & j, const LastCardMsg & x);

void from_json(const json & j, LayDownMsg & x);
void to_json(json & j, const LayDownMsg & x);

void from_json(const json & j, PlayCardMsg & x);
void to_json(json & j, const PlayCardMsg & x);

void from_json(const json & j, WinTrickMsg & x);
void to_json(json & j, const WinTrickMsg & x);

void from_json(const json & j, WinHandMsg & x);
void to_json(json & j, const WinHandMsg & x);

void from_json(const json & j, WinGameMsg & x);
void to_json(json & j, const WinGameMsg & x);

void from_json(const json & j, CookieMsg & x);
void to_json(json & j, const CookieMsg & x);

void from_json(const json & j, ServerDiscardMsg & x);
void to_json(json & j, const ServerDiscardMsg & x);

void from_json(const json & j, DiscardMsg & x);
void to_json(json & j, const DiscardMsg & x);

void from_json(const json & j, GameError & x);
void to_json(json & j, const GameError & x);

void from_json(const json & j, Redirect & x);
void to_json(json & j, const Redirect & x);

void from_json(const json & j, ServerPlayer & x);
void to_json(json & j, const ServerPlayer & x);

void from_json(const json & j, GameState & x);
void to_json(json & j, const GameState & x);

void from_json(const json & j, DealCardsMsg & x);
void to_json(json & j, const DealCardsMsg & x);

void from_json(const json & j, WelcomeMsg & x);
void to_json(json & j, const WelcomeMsg & x);

void from_json(const json & j, UndoPremoveMsg & x);
void to_json(json & j, const UndoPremoveMsg & x);

void from_json(const json & j, PremoveMsg & x);
void to_json(json & j, const PremoveMsg & x);

void from_json(const json & j, RestartMsg & x);
void to_json(json & j, const RestartMsg & x);

void from_json(const json & j, JoinMsg & x);
void to_json(json & j, const JoinMsg & x);

void from_json(const json & j, DisconnectMsg & x);
void to_json(json & j, const DisconnectMsg & x);

void from_json(const json & j, ReconnectMsg & x);
void to_json(json & j, const ReconnectMsg & x);

void from_json(const json & j, UpdateNameMsg & x);
void to_json(json & j, const UpdateNameMsg & x);

void from_json(const json & j, PlayerUpdateMsg & x);
void to_json(json & j, const PlayerUpdateMsg & x);

void from_json(const json & j, UpdateMsg & x);
void to_json(json & j, const UpdateMsg & x);

void from_json(const json & j, Rank & x);
void to_json(json & j, const Rank & x);

void from_json(const json & j, Suit & x);
void to_json(json & j, const Suit & x);

void from_json(const json & j, ClientMsgType & x);
void to_json(json & j, const ClientMsgType & x);

void from_json(const json & j, Alignment & x);
void to_json(json & j, const Alignment & x);

void from_json(const json & j, Modifier & x);
void to_json(json & j, const Modifier & x);

void from_json(const json & j, RichTextChunkType & x);
void to_json(json & j, const RichTextChunkType & x);

void from_json(const json & j, Phase & x);
void to_json(json & j, const Phase & x);

void from_json(const json & j, RichTextMsgType & x);
void to_json(json & j, const RichTextMsgType & x);

void from_json(const json & j, ServerMsgType & x);
void to_json(json & j, const ServerMsgType & x);

void from_json(const json & j, PassMsgType & x);
void to_json(json & j, const PassMsgType & x);

void from_json(const json & j, PlayJajaDingDongMsgType & x);
void to_json(json & j, const PlayJajaDingDongMsgType & x);

void from_json(const json & j, TableTalkMsgType & x);
void to_json(json & j, const TableTalkMsgType & x);

void from_json(const json & j, OrderMsgType & x);
void to_json(json & j, const OrderMsgType & x);

void from_json(const json & j, LastCardMsgType & x);
void to_json(json & j, const LastCardMsgType & x);

void from_json(const json & j, LayDownMsgType & x);
void to_json(json & j, const LayDownMsgType & x);

void from_json(const json & j, PlayCardMsgType & x);
void to_json(json & j, const PlayCardMsgType & x);

void from_json(const json & j, WinTrickMsgType & x);
void to_json(json & j, const WinTrickMsgType & x);

void from_json(const json & j, WinHandMsgType & x);
void to_json(json & j, const WinHandMsgType & x);

void from_json(const json & j, WinGameMsgType & x);
void to_json(json & j, const WinGameMsgType & x);

void from_json(const json & j, CookieMsgType & x);
void to_json(json & j, const CookieMsgType & x);

void from_json(const json & j, ServerDiscardMsgType & x);
void to_json(json & j, const ServerDiscardMsgType & x);

void from_json(const json & j, DiscardMsgType & x);
void to_json(json & j, const DiscardMsgType & x);

void from_json(const json & j, GameErrorType & x);
void to_json(json & j, const GameErrorType & x);

void from_json(const json & j, RedirectType & x);
void to_json(json & j, const RedirectType & x);

void from_json(const json & j, GameStateType & x);
void to_json(json & j, const GameStateType & x);

void from_json(const json & j, DealCardsMsgType & x);
void to_json(json & j, const DealCardsMsgType & x);

void from_json(const json & j, WelcomeMsgType & x);
void to_json(json & j, const WelcomeMsgType & x);

void from_json(const json & j, UndoPremoveMsgType & x);
void to_json(json & j, const UndoPremoveMsgType & x);

void from_json(const json & j, PremoveMsgType & x);
void to_json(json & j, const PremoveMsgType & x);

void from_json(const json & j, RestartMsgType & x);
void to_json(json & j, const RestartMsgType & x);

void from_json(const json & j, JoinMsgType & x);
void to_json(json & j, const JoinMsgType & x);

void from_json(const json & j, DisconnectMsgType & x);
void to_json(json & j, const DisconnectMsgType & x);

void from_json(const json & j, ReconnectMsgType & x);
void to_json(json & j, const ReconnectMsgType & x);

void from_json(const json & j, UpdateNameMsgType & x);
void to_json(json & j, const UpdateNameMsgType & x);

void from_json(const json & j, PlayerUpdateMsgType & x);
void to_json(json & j, const PlayerUpdateMsgType & x);

void from_json(const json & j, UpdateMsgType & x);
void to_json(json & j, const UpdateMsgType & x);
}
namespace nlohmann {
template <>
struct adl_serializer<std::variant<API::RichTextChunk, std::string>> {
    static void from_json(const json & j, std::variant<API::RichTextChunk, std::string> & x);
    static void to_json(json & j, const std::variant<API::RichTextChunk, std::string> & x);
};
}
namespace API {
    inline void from_json(const json & j, Card& x) {
        x.illegal = get_stack_optional<bool>(j, "illegal");
        x.rank = j.at("rank").get<Rank>();
        x.suit = j.at("suit").get<Suit>();
    }

    inline void to_json(json & j, const Card & x) {
        j = json::object();
        if (x.illegal) {
            j["illegal"] = x.illegal;
        }
        j["rank"] = x.rank;
        j["suit"] = x.suit;
    }

    inline void from_json(const json & j, ClientMsg& x) {
        x.session = get_stack_optional<std::string>(j, "session");
        x.type = j.at("type").get<ClientMsgType>();
        x.card = get_stack_optional<Card>(j, "card");
        x.id = get_stack_optional<int64_t>(j, "id");
        x.alone = get_stack_optional<bool>(j, "alone");
        x.suit = get_stack_optional<Suit>(j, "suit");
        x.table_talk = get_stack_optional<int64_t>(j, "table_talk");
        x.name = get_stack_optional<std::string>(j, "name");
        x.premoves = get_stack_optional<int64_t>(j, "premoves");
        x.cards = get_stack_optional<std::vector<Card>>(j, "cards");
    }

    inline void to_json(json & j, const ClientMsg & x) {
        j = json::object();
        if (x.session) {
            j["session"] = x.session;
        }
        j["type"] = x.type;
        if (x.card) {
            j["card"] = x.card;
        }
        if (x.id) {
            j["id"] = x.id;
        }
        if (x.alone) {
            j["alone"] = x.alone;
        }
        if (x.suit) {
            j["suit"] = x.suit;
        }
        if (x.table_talk) {
            j["table_talk"] = x.table_talk;
        }
        if (x.name) {
            j["name"] = x.name;
        }
        if (x.premoves) {
            j["premoves"] = x.premoves;
        }
        if (x.cards) {
            j["cards"] = x.cards;
        }
    }

    inline void from_json(const json & j, CardElement& x) {
        x.card = get_stack_optional<Card>(j, "card");
        x.id = get_stack_optional<int64_t>(j, "id");
        x.illegal = get_stack_optional<bool>(j, "illegal");
        x.rank = get_stack_optional<Rank>(j, "rank");
        x.suit = get_stack_optional<Suit>(j, "suit");
    }

    inline void to_json(json & j, const CardElement & x) {
        j = json::object();
        if (x.card) {
            j["card"] = x.card;
        }
        if (x.id) {
            j["id"] = x.id;
        }
        if (x.illegal) {
            j["illegal"] = x.illegal;
        }
        if (x.rank) {
            j["rank"] = x.rank;
        }
        if (x.suit) {
            j["suit"] = x.suit;
        }
    }

    inline void from_json(const json & j, RichTextChunk& x) {
        x.alignment = get_stack_optional<Alignment>(j, "alignment");
        x.color = get_stack_optional<std::string>(j, "color");
        x.modifiers = get_stack_optional<std::vector<Modifier>>(j, "modifiers");
        x.text = j.at("text").get<std::string>();
        x.type = j.at("type").get<RichTextChunkType>();
    }

    inline void to_json(json & j, const RichTextChunk & x) {
        j = json::object();
        if (x.alignment) {
            j["alignment"] = x.alignment;
        }
        if (x.color) {
            j["color"] = x.color;
        }
        if (x.modifiers) {
            j["modifiers"] = x.modifiers;
        }
        j["text"] = x.text;
        j["type"] = x.type;
    }

    inline void from_json(const json & j, TaggedCard& x) {
        x.card = j.at("card").get<Card>();
        x.id = j.at("id").get<int64_t>();
    }

    inline void to_json(json & j, const TaggedCard & x) {
        j = json::object();
        j["card"] = x.card;
        j["id"] = x.id;
    }

    inline void from_json(const json & j, Player& x) {
        x.card_count = j.at("card_count").get<int64_t>();
        x.connected = j.at("connected").get<bool>();
        x.name = get_stack_optional<std::string>(j, "name");
        x.premoved = j.at("premoved").get<int64_t>();
        x.sitting_out = j.at("sitting_out").get<bool>();
        x.tricks = j.at("tricks").get<int64_t>();
    }

    inline void to_json(json & j, const Player & x) {
        j = json::object();
        j["card_count"] = x.card_count;
        j["connected"] = x.connected;
        if (x.name) {
            j["name"] = x.name;
        }
        j["premoved"] = x.premoved;
        j["sitting_out"] = x.sitting_out;
        j["tricks"] = x.tricks;
    }

    inline void from_json(const json & j, RichTextMsg& x) {
        x.msg = j.at("msg").get<std::vector<Msg>>();
        x.type = j.at("type").get<RichTextMsgType>();
    }

    inline void to_json(json & j, const RichTextMsg & x) {
        j = json::object();
        j["msg"] = x.msg;
        j["type"] = x.type;
    }

    inline void from_json(const json & j, ServerMsg& x) {
        x.error = get_stack_optional<std::string>(j, "error");
        x.type = j.at("type").get<ServerMsgType>();
        x.id = get_stack_optional<int64_t>(j, "id");
        x.room = get_stack_optional<std::string>(j, "room");
        x.dealer = get_stack_optional<int64_t>(j, "dealer");
        x.phase = get_stack_optional<Phase>(j, "phase");
        x.played_cards = get_stack_optional<std::vector<TaggedCard>>(j, "played_cards");
        x.players = get_stack_optional<std::vector<Player>>(j, "players");
        x.private_session = get_stack_optional<bool>(j, "private_session");
        x.rich_chat_log = get_stack_optional<std::vector<RichTextMsg>>(j, "rich_chat_log");
        x.scores = get_stack_optional<std::vector<int64_t>>(j, "scores");
        x.top_card = get_stack_optional<Card>(j, "top_card");
        x.trick = get_stack_optional<std::vector<Card>>(j, "trick");
        x.trump = get_stack_optional<Suit>(j, "trump");
        x.turn = get_stack_optional<int64_t>(j, "turn");
        x.your_cards = get_stack_optional<std::vector<Card>>(j, "your_cards");
        x.your_premoves = get_stack_optional<std::vector<Card>>(j, "your_premoves");
        x.alone = get_stack_optional<bool>(j, "alone");
        x.suit = get_stack_optional<Suit>(j, "suit");
        x.table_talk = get_stack_optional<int64_t>(j, "table_talk");
        x.card = get_stack_optional<Card>(j, "card");
        x.name = get_stack_optional<std::string>(j, "name");
        x.msg = get_stack_optional<std::vector<Msg>>(j, "msg");
        x.cards = get_stack_optional<std::vector<CardElement>>(j, "cards");
    }

    inline void to_json(json & j, const ServerMsg & x) {
        j = json::object();
        if (x.error) {
            j["error"] = x.error;
        }
        j["type"] = x.type;
        if (x.id) {
            j["id"] = x.id;
        }
        if (x.room) {
            j["room"] = x.room;
        }
        if (x.dealer) {
            j["dealer"] = x.dealer;
        }
        if (x.phase) {
            j["phase"] = x.phase;
        }
        if (x.played_cards) {
            j["played_cards"] = x.played_cards;
        }
        if (x.players) {
            j["players"] = x.players;
        }
        if (x.private_session) {
            j["private_session"] = x.private_session;
        }
        if (x.rich_chat_log) {
            j["rich_chat_log"] = x.rich_chat_log;
        }
        if (x.scores) {
            j["scores"] = x.scores;
        }
        if (x.top_card) {
            j["top_card"] = x.top_card;
        }
        if (x.trick) {
            j["trick"] = x.trick;
        }
        if (x.trump) {
            j["trump"] = x.trump;
        }
        if (x.turn) {
            j["turn"] = x.turn;
        }
        if (x.your_cards) {
            j["your_cards"] = x.your_cards;
        }
        if (x.your_premoves) {
            j["your_premoves"] = x.your_premoves;
        }
        if (x.alone) {
            j["alone"] = x.alone;
        }
        if (x.suit) {
            j["suit"] = x.suit;
        }
        if (x.table_talk) {
            j["table_talk"] = x.table_talk;
        }
        if (x.card) {
            j["card"] = x.card;
        }
        if (x.name) {
            j["name"] = x.name;
        }
        if (x.msg) {
            j["msg"] = x.msg;
        }
        if (x.cards) {
            j["cards"] = x.cards;
        }
    }

    inline void from_json(const json & j, PassMsg& x) {
        x.id = get_stack_optional<int64_t>(j, "id");
        x.type = j.at("type").get<PassMsgType>();
    }

    inline void to_json(json & j, const PassMsg & x) {
        j = json::object();
        if (x.id) {
            j["id"] = x.id;
        }
        j["type"] = x.type;
    }

    inline void from_json(const json & j, PlayJajaDingDongMsg& x) {
        x.id = get_stack_optional<int64_t>(j, "id");
        x.type = j.at("type").get<PlayJajaDingDongMsgType>();
    }

    inline void to_json(json & j, const PlayJajaDingDongMsg & x) {
        j = json::object();
        if (x.id) {
            j["id"] = x.id;
        }
        j["type"] = x.type;
    }

    inline void from_json(const json & j, TableTalkMsg& x) {
        x.id = get_stack_optional<int64_t>(j, "id");
        x.table_talk = j.at("table_talk").get<int64_t>();
        x.type = j.at("type").get<TableTalkMsgType>();
    }

    inline void to_json(json & j, const TableTalkMsg & x) {
        j = json::object();
        if (x.id) {
            j["id"] = x.id;
        }
        j["table_talk"] = x.table_talk;
        j["type"] = x.type;
    }

    inline void from_json(const json & j, OrderMsg& x) {
        x.alone = get_stack_optional<bool>(j, "alone");
        x.id = get_stack_optional<int64_t>(j, "id");
        x.suit = get_stack_optional<Suit>(j, "suit");
        x.type = j.at("type").get<OrderMsgType>();
    }

    inline void to_json(json & j, const OrderMsg & x) {
        j = json::object();
        if (x.alone) {
            j["alone"] = x.alone;
        }
        if (x.id) {
            j["id"] = x.id;
        }
        if (x.suit) {
            j["suit"] = x.suit;
        }
        j["type"] = x.type;
    }

    inline void from_json(const json & j, LastCardMsg& x) {
        x.cards = j.at("cards").get<std::vector<TaggedCard>>();
        x.type = j.at("type").get<LastCardMsgType>();
    }

    inline void to_json(json & j, const LastCardMsg & x) {
        j = json::object();
        j["cards"] = x.cards;
        j["type"] = x.type;
    }

    inline void from_json(const json & j, LayDownMsg& x) {
        x.cards = get_stack_optional<std::vector<Card>>(j, "cards");
        x.id = get_stack_optional<int64_t>(j, "id");
        x.type = j.at("type").get<LayDownMsgType>();
    }

    inline void to_json(json & j, const LayDownMsg & x) {
        j = json::object();
        if (x.cards) {
            j["cards"] = x.cards;
        }
        if (x.id) {
            j["id"] = x.id;
        }
        j["type"] = x.type;
    }

    inline void from_json(const json & j, PlayCardMsg& x) {
        x.card = j.at("card").get<Card>();
        x.id = get_stack_optional<int64_t>(j, "id");
        x.type = j.at("type").get<PlayCardMsgType>();
    }

    inline void to_json(json & j, const PlayCardMsg & x) {
        j = json::object();
        j["card"] = x.card;
        if (x.id) {
            j["id"] = x.id;
        }
        j["type"] = x.type;
    }

    inline void from_json(const json & j, WinTrickMsg& x) {
        x.id = j.at("id").get<int64_t>();
        x.type = j.at("type").get<WinTrickMsgType>();
    }

    inline void to_json(json & j, const WinTrickMsg & x) {
        j = json::object();
        j["id"] = x.id;
        j["type"] = x.type;
    }

    inline void from_json(const json & j, WinHandMsg& x) {
        x.id = j.at("id").get<int64_t>();
        x.type = j.at("type").get<WinHandMsgType>();
    }

    inline void to_json(json & j, const WinHandMsg & x) {
        j = json::object();
        j["id"] = x.id;
        j["type"] = x.type;
    }

    inline void from_json(const json & j, WinGameMsg& x) {
        x.id = j.at("id").get<int64_t>();
        x.type = j.at("type").get<WinGameMsgType>();
    }

    inline void to_json(json & j, const WinGameMsg & x) {
        j = json::object();
        j["id"] = x.id;
        j["type"] = x.type;
    }

    inline void from_json(const json & j, CookieMsg& x) {
        x.session = j.at("session").get<std::string>();
        x.type = j.at("type").get<CookieMsgType>();
    }

    inline void to_json(json & j, const CookieMsg & x) {
        j = json::object();
        j["session"] = x.session;
        j["type"] = x.type;
    }

    inline void from_json(const json & j, ServerDiscardMsg& x) {
        x.id = j.at("id").get<int64_t>();
        x.type = j.at("type").get<ServerDiscardMsgType>();
    }

    inline void to_json(json & j, const ServerDiscardMsg & x) {
        j = json::object();
        j["id"] = x.id;
        j["type"] = x.type;
    }

    inline void from_json(const json & j, DiscardMsg& x) {
        x.card = j.at("card").get<Card>();
        x.id = get_stack_optional<int64_t>(j, "id");
        x.type = j.at("type").get<DiscardMsgType>();
    }

    inline void to_json(json & j, const DiscardMsg & x) {
        j = json::object();
        j["card"] = x.card;
        if (x.id) {
            j["id"] = x.id;
        }
        j["type"] = x.type;
    }

    inline void from_json(const json & j, GameError& x) {
        x.error = j.at("error").get<std::string>();
        x.type = j.at("type").get<GameErrorType>();
    }

    inline void to_json(json & j, const GameError & x) {
        j = json::object();
        j["error"] = x.error;
        j["type"] = x.type;
    }

    inline void from_json(const json & j, Redirect& x) {
        x.room = j.at("room").get<std::string>();
        x.type = j.at("type").get<RedirectType>();
    }

    inline void to_json(json & j, const Redirect & x) {
        j = json::object();
        j["room"] = x.room;
        j["type"] = x.type;
    }

    inline void from_json(const json & j, ServerPlayer& x) {
        x.bot_enum = j.at("bot_enum").get<int64_t>();
        x.cards = j.at("cards").get<std::vector<Card>>();
        x.connected = j.at("connected").get<bool>();
        x.name = get_stack_optional<std::string>(j, "name");
        x.premoves = j.at("premoves").get<std::vector<Card>>();
        x.session = j.at("session").get<std::string>();
        x.sitting_out = j.at("sitting_out").get<bool>();
        x.tricks = j.at("tricks").get<int64_t>();
    }

    inline void to_json(json & j, const ServerPlayer & x) {
        j = json::object();
        j["bot_enum"] = x.bot_enum;
        j["cards"] = x.cards;
        j["connected"] = x.connected;
        if (x.name) {
            j["name"] = x.name;
        }
        j["premoves"] = x.premoves;
        j["session"] = x.session;
        j["sitting_out"] = x.sitting_out;
        j["tricks"] = x.tricks;
    }

    inline void from_json(const json & j, GameState& x) {
        x.caller = j.at("caller").get<int64_t>();
        x.dealer = j.at("dealer").get<int64_t>();
        x.kitty = get_stack_optional<std::vector<Card>>(j, "kitty");
        x.phase = j.at("phase").get<Phase>();
        x.played_cards = j.at("played_cards").get<std::vector<TaggedCard>>();
        x.players = j.at("players").get<std::vector<ServerPlayer>>();
        x.private_session = j.at("private_session").get<bool>();
        x.rich_chat_log = j.at("rich_chat_log").get<std::vector<RichTextMsg>>();
        x.scores = j.at("scores").get<std::vector<int64_t>>();
        x.top_card = get_stack_optional<Card>(j, "top_card");
        x.trick = j.at("trick").get<std::vector<Card>>();
        x.trick_leader = j.at("trick_leader").get<int64_t>();
        x.trump = j.at("trump").get<Suit>();
        x.turn = j.at("turn").get<int64_t>();
        x.type = j.at("type").get<GameStateType>();
    }

    inline void to_json(json & j, const GameState & x) {
        j = json::object();
        j["caller"] = x.caller;
        j["dealer"] = x.dealer;
        if (x.kitty) {
            j["kitty"] = x.kitty;
        }
        j["phase"] = x.phase;
        j["played_cards"] = x.played_cards;
        j["players"] = x.players;
        j["private_session"] = x.private_session;
        j["rich_chat_log"] = x.rich_chat_log;
        j["scores"] = x.scores;
        if (x.top_card) {
            j["top_card"] = x.top_card;
        }
        j["trick"] = x.trick;
        j["trick_leader"] = x.trick_leader;
        j["trump"] = x.trump;
        j["turn"] = x.turn;
        j["type"] = x.type;
    }

    inline void from_json(const json & j, DealCardsMsg& x) {
        x.top_card = j.at("top_card").get<Card>();
        x.type = j.at("type").get<DealCardsMsgType>();
        x.your_cards = j.at("your_cards").get<std::vector<Card>>();
    }

    inline void to_json(json & j, const DealCardsMsg & x) {
        j = json::object();
        j["top_card"] = x.top_card;
        j["type"] = x.type;
        j["your_cards"] = x.your_cards;
    }

    inline void from_json(const json & j, WelcomeMsg& x) {
        x.dealer = j.at("dealer").get<int64_t>();
        x.id = j.at("id").get<int64_t>();
        x.phase = j.at("phase").get<Phase>();
        x.played_cards = j.at("played_cards").get<std::vector<TaggedCard>>();
        x.players = j.at("players").get<std::vector<Player>>();
        x.private_session = j.at("private_session").get<bool>();
        x.rich_chat_log = j.at("rich_chat_log").get<std::vector<RichTextMsg>>();
        x.scores = j.at("scores").get<std::vector<int64_t>>();
        x.top_card = get_stack_optional<Card>(j, "top_card");
        x.trick = j.at("trick").get<std::vector<Card>>();
        x.trump = j.at("trump").get<Suit>();
        x.turn = j.at("turn").get<int64_t>();
        x.type = j.at("type").get<WelcomeMsgType>();
        x.your_cards = j.at("your_cards").get<std::vector<Card>>();
        x.your_premoves = j.at("your_premoves").get<std::vector<Card>>();
    }

    inline void to_json(json & j, const WelcomeMsg & x) {
        j = json::object();
        j["dealer"] = x.dealer;
        j["id"] = x.id;
        j["phase"] = x.phase;
        j["played_cards"] = x.played_cards;
        j["players"] = x.players;
        j["private_session"] = x.private_session;
        j["rich_chat_log"] = x.rich_chat_log;
        j["scores"] = x.scores;
        if (x.top_card) {
            j["top_card"] = x.top_card;
        }
        j["trick"] = x.trick;
        j["trump"] = x.trump;
        j["turn"] = x.turn;
        j["type"] = x.type;
        j["your_cards"] = x.your_cards;
        j["your_premoves"] = x.your_premoves;
    }

    inline void from_json(const json & j, UndoPremoveMsg& x) {
        x.type = j.at("type").get<UndoPremoveMsgType>();
    }

    inline void to_json(json & j, const UndoPremoveMsg & x) {
        j = json::object();
        j["type"] = x.type;
    }

    inline void from_json(const json & j, PremoveMsg& x) {
        x.card = j.at("card").get<Card>();
        x.type = j.at("type").get<PremoveMsgType>();
    }

    inline void to_json(json & j, const PremoveMsg & x) {
        j = json::object();
        j["card"] = x.card;
        j["type"] = x.type;
    }

    inline void from_json(const json & j, RestartMsg& x) {
        x.id = get_stack_optional<int64_t>(j, "id");
        x.type = j.at("type").get<RestartMsgType>();
    }

    inline void to_json(json & j, const RestartMsg & x) {
        j = json::object();
        if (x.id) {
            j["id"] = x.id;
        }
        j["type"] = x.type;
    }

    inline void from_json(const json & j, JoinMsg& x) {
        x.id = j.at("id").get<int64_t>();
        x.name = get_stack_optional<std::string>(j, "name");
        x.type = j.at("type").get<JoinMsgType>();
    }

    inline void to_json(json & j, const JoinMsg & x) {
        j = json::object();
        j["id"] = x.id;
        if (x.name) {
            j["name"] = x.name;
        }
        j["type"] = x.type;
    }

    inline void from_json(const json & j, DisconnectMsg& x) {
        x.id = j.at("id").get<int64_t>();
        x.type = j.at("type").get<DisconnectMsgType>();
    }

    inline void to_json(json & j, const DisconnectMsg & x) {
        j = json::object();
        j["id"] = x.id;
        j["type"] = x.type;
    }

    inline void from_json(const json & j, ReconnectMsg& x) {
        x.id = j.at("id").get<int64_t>();
        x.type = j.at("type").get<ReconnectMsgType>();
    }

    inline void to_json(json & j, const ReconnectMsg & x) {
        j = json::object();
        j["id"] = x.id;
        j["type"] = x.type;
    }

    inline void from_json(const json & j, UpdateNameMsg& x) {
        x.name = j.at("name").get<std::string>();
        x.type = j.at("type").get<UpdateNameMsgType>();
    }

    inline void to_json(json & j, const UpdateNameMsg & x) {
        j = json::object();
        j["name"] = x.name;
        j["type"] = x.type;
    }

    inline void from_json(const json & j, PlayerUpdateMsg& x) {
        x.id = j.at("id").get<int64_t>();
        x.name = get_stack_optional<std::string>(j, "name");
        x.premoves = get_stack_optional<int64_t>(j, "premoves");
        x.type = j.at("type").get<PlayerUpdateMsgType>();
    }

    inline void to_json(json & j, const PlayerUpdateMsg & x) {
        j = json::object();
        j["id"] = x.id;
        if (x.name) {
            j["name"] = x.name;
        }
        if (x.premoves) {
            j["premoves"] = x.premoves;
        }
        j["type"] = x.type;
    }

    inline void from_json(const json & j, UpdateMsg& x) {
        x.dealer = j.at("dealer").get<int64_t>();
        x.phase = j.at("phase").get<Phase>();
        x.scores = j.at("scores").get<std::vector<int64_t>>();
        x.trump = j.at("trump").get<Suit>();
        x.turn = j.at("turn").get<int64_t>();
        x.type = j.at("type").get<UpdateMsgType>();
    }

    inline void to_json(json & j, const UpdateMsg & x) {
        j = json::object();
        j["dealer"] = x.dealer;
        j["phase"] = x.phase;
        j["scores"] = x.scores;
        j["trump"] = x.trump;
        j["turn"] = x.turn;
        j["type"] = x.type;
    }

    inline void from_json(const json & j, Rank & x) {
        if (j == "ace") x = Rank::ACE;
        else if (j == "jack") x = Rank::JACK;
        else if (j == "king") x = Rank::KING;
        else if (j == "nine") x = Rank::NINE;
        else if (j == "queen") x = Rank::QUEEN;
        else if (j == "ten") x = Rank::TEN;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const Rank & x) {
        switch (x) {
            case Rank::ACE: j = "ace"; break;
            case Rank::JACK: j = "jack"; break;
            case Rank::KING: j = "king"; break;
            case Rank::NINE: j = "nine"; break;
            case Rank::QUEEN: j = "queen"; break;
            case Rank::TEN: j = "ten"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, Suit & x) {
        if (j == "clubs") x = Suit::CLUBS;
        else if (j == "diamonds") x = Suit::DIAMONDS;
        else if (j == "hearts") x = Suit::HEARTS;
        else if (j == "spades") x = Suit::SPADES;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const Suit & x) {
        switch (x) {
            case Suit::CLUBS: j = "clubs"; break;
            case Suit::DIAMONDS: j = "diamonds"; break;
            case Suit::HEARTS: j = "hearts"; break;
            case Suit::SPADES: j = "spades"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, ClientMsgType & x) {
        if (j == "cookie") x = ClientMsgType::COOKIE;
        else if (j == "discard") x = ClientMsgType::DISCARD;
        else if (j == "lay_down") x = ClientMsgType::LAY_DOWN;
        else if (j == "order") x = ClientMsgType::ORDER;
        else if (j == "pass") x = ClientMsgType::PASS;
        else if (j == "play_card") x = ClientMsgType::PLAY_CARD;
        else if (j == "play_jaja_ding_dong") x = ClientMsgType::PLAY_JAJA_DING_DONG;
        else if (j == "premove") x = ClientMsgType::PREMOVE;
        else if (j == "restart") x = ClientMsgType::RESTART;
        else if (j == "table_talk") x = ClientMsgType::TABLE_TALK;
        else if (j == "undo_premove") x = ClientMsgType::UNDO_PREMOVE;
        else if (j == "update_player") x = ClientMsgType::UPDATE_PLAYER;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const ClientMsgType & x) {
        switch (x) {
            case ClientMsgType::COOKIE: j = "cookie"; break;
            case ClientMsgType::DISCARD: j = "discard"; break;
            case ClientMsgType::LAY_DOWN: j = "lay_down"; break;
            case ClientMsgType::ORDER: j = "order"; break;
            case ClientMsgType::PASS: j = "pass"; break;
            case ClientMsgType::PLAY_CARD: j = "play_card"; break;
            case ClientMsgType::PLAY_JAJA_DING_DONG: j = "play_jaja_ding_dong"; break;
            case ClientMsgType::PREMOVE: j = "premove"; break;
            case ClientMsgType::RESTART: j = "restart"; break;
            case ClientMsgType::TABLE_TALK: j = "table_talk"; break;
            case ClientMsgType::UNDO_PREMOVE: j = "undo_premove"; break;
            case ClientMsgType::UPDATE_PLAYER: j = "update_player"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, Alignment & x) {
        if (j == "center") x = Alignment::CENTER;
        else if (j == "left") x = Alignment::LEFT;
        else if (j == "right") x = Alignment::RIGHT;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const Alignment & x) {
        switch (x) {
            case Alignment::CENTER: j = "center"; break;
            case Alignment::LEFT: j = "left"; break;
            case Alignment::RIGHT: j = "right"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, Modifier & x) {
        if (j == "bold") x = Modifier::BOLD;
        else if (j == "italic") x = Modifier::ITALIC;
        else if (j == "strikethrough") x = Modifier::STRIKETHROUGH;
        else if (j == "underline") x = Modifier::UNDERLINE;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const Modifier & x) {
        switch (x) {
            case Modifier::BOLD: j = "bold"; break;
            case Modifier::ITALIC: j = "italic"; break;
            case Modifier::STRIKETHROUGH: j = "strikethrough"; break;
            case Modifier::UNDERLINE: j = "underline"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, RichTextChunkType & x) {
        if (j == "rt_text") x = RichTextChunkType::RT_TEXT;
        else if (j == "rt_username") x = RichTextChunkType::RT_USERNAME;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const RichTextChunkType & x) {
        switch (x) {
            case RichTextChunkType::RT_TEXT: j = "rt_text"; break;
            case RichTextChunkType::RT_USERNAME: j = "rt_username"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, Phase & x) {
        if (j == "discarding") x = Phase::DISCARDING;
        else if (j == "ended") x = Phase::ENDED;
        else if (j == "lobby") x = Phase::LOBBY;
        else if (j == "playing") x = Phase::PLAYING;
        else if (j == "vote_round1") x = Phase::VOTE_ROUND1;
        else if (j == "vote_round2") x = Phase::VOTE_ROUND2;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const Phase & x) {
        switch (x) {
            case Phase::DISCARDING: j = "discarding"; break;
            case Phase::ENDED: j = "ended"; break;
            case Phase::LOBBY: j = "lobby"; break;
            case Phase::PLAYING: j = "playing"; break;
            case Phase::VOTE_ROUND1: j = "vote_round1"; break;
            case Phase::VOTE_ROUND2: j = "vote_round2"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, RichTextMsgType & x) {
        if (j == "chat") x = RichTextMsgType::CHAT;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const RichTextMsgType & x) {
        switch (x) {
            case RichTextMsgType::CHAT: j = "chat"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, ServerMsgType & x) {
        static std::unordered_map<std::string, ServerMsgType> enumValues {
            {"chat", ServerMsgType::CHAT},
            {"deal", ServerMsgType::DEAL},
            {"discarded", ServerMsgType::DISCARDED},
            {"disconnect", ServerMsgType::DISCONNECT},
            {"error", ServerMsgType::ERROR},
            {"join", ServerMsgType::JOIN},
            {"last_card", ServerMsgType::LAST_CARD},
            {"lay_down", ServerMsgType::LAY_DOWN},
            {"order", ServerMsgType::ORDER},
            {"pass", ServerMsgType::PASS},
            {"play_card", ServerMsgType::PLAY_CARD},
            {"play_jaja_ding_dong", ServerMsgType::PLAY_JAJA_DING_DONG},
            {"reconnect", ServerMsgType::RECONNECT},
            {"redirect", ServerMsgType::REDIRECT},
            {"table_talk", ServerMsgType::TABLE_TALK},
            {"update", ServerMsgType::UPDATE},
            {"update_name", ServerMsgType::UPDATE_NAME},
            {"welcome", ServerMsgType::WELCOME},
        };
        auto iter = enumValues.find(j.get<std::string>());
        if (iter != enumValues.end()) {
            x = iter->second;
        }
    }

    inline void to_json(json & j, const ServerMsgType & x) {
        switch (x) {
            case ServerMsgType::CHAT: j = "chat"; break;
            case ServerMsgType::DEAL: j = "deal"; break;
            case ServerMsgType::DISCARDED: j = "discarded"; break;
            case ServerMsgType::DISCONNECT: j = "disconnect"; break;
            case ServerMsgType::ERROR: j = "error"; break;
            case ServerMsgType::JOIN: j = "join"; break;
            case ServerMsgType::LAST_CARD: j = "last_card"; break;
            case ServerMsgType::LAY_DOWN: j = "lay_down"; break;
            case ServerMsgType::ORDER: j = "order"; break;
            case ServerMsgType::PASS: j = "pass"; break;
            case ServerMsgType::PLAY_CARD: j = "play_card"; break;
            case ServerMsgType::PLAY_JAJA_DING_DONG: j = "play_jaja_ding_dong"; break;
            case ServerMsgType::RECONNECT: j = "reconnect"; break;
            case ServerMsgType::REDIRECT: j = "redirect"; break;
            case ServerMsgType::TABLE_TALK: j = "table_talk"; break;
            case ServerMsgType::UPDATE: j = "update"; break;
            case ServerMsgType::UPDATE_NAME: j = "update_name"; break;
            case ServerMsgType::WELCOME: j = "welcome"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, PassMsgType & x) {
        if (j == "pass") x = PassMsgType::PASS;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const PassMsgType & x) {
        switch (x) {
            case PassMsgType::PASS: j = "pass"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, PlayJajaDingDongMsgType & x) {
        if (j == "play_jaja_ding_dong") x = PlayJajaDingDongMsgType::PLAY_JAJA_DING_DONG;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const PlayJajaDingDongMsgType & x) {
        switch (x) {
            case PlayJajaDingDongMsgType::PLAY_JAJA_DING_DONG: j = "play_jaja_ding_dong"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, TableTalkMsgType & x) {
        if (j == "table_talk") x = TableTalkMsgType::TABLE_TALK;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const TableTalkMsgType & x) {
        switch (x) {
            case TableTalkMsgType::TABLE_TALK: j = "table_talk"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, OrderMsgType & x) {
        if (j == "order") x = OrderMsgType::ORDER;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const OrderMsgType & x) {
        switch (x) {
            case OrderMsgType::ORDER: j = "order"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, LastCardMsgType & x) {
        if (j == "last_card") x = LastCardMsgType::LAST_CARD;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const LastCardMsgType & x) {
        switch (x) {
            case LastCardMsgType::LAST_CARD: j = "last_card"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, LayDownMsgType & x) {
        if (j == "lay_down") x = LayDownMsgType::LAY_DOWN;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const LayDownMsgType & x) {
        switch (x) {
            case LayDownMsgType::LAY_DOWN: j = "lay_down"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, PlayCardMsgType & x) {
        if (j == "play_card") x = PlayCardMsgType::PLAY_CARD;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const PlayCardMsgType & x) {
        switch (x) {
            case PlayCardMsgType::PLAY_CARD: j = "play_card"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, WinTrickMsgType & x) {
        if (j == "win_trick") x = WinTrickMsgType::WIN_TRICK;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const WinTrickMsgType & x) {
        switch (x) {
            case WinTrickMsgType::WIN_TRICK: j = "win_trick"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, WinHandMsgType & x) {
        if (j == "win_hand") x = WinHandMsgType::WIN_HAND;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const WinHandMsgType & x) {
        switch (x) {
            case WinHandMsgType::WIN_HAND: j = "win_hand"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, WinGameMsgType & x) {
        if (j == "win_game") x = WinGameMsgType::WIN_GAME;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const WinGameMsgType & x) {
        switch (x) {
            case WinGameMsgType::WIN_GAME: j = "win_game"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, CookieMsgType & x) {
        if (j == "cookie") x = CookieMsgType::COOKIE;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const CookieMsgType & x) {
        switch (x) {
            case CookieMsgType::COOKIE: j = "cookie"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, ServerDiscardMsgType & x) {
        if (j == "discarded") x = ServerDiscardMsgType::DISCARDED;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const ServerDiscardMsgType & x) {
        switch (x) {
            case ServerDiscardMsgType::DISCARDED: j = "discarded"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, DiscardMsgType & x) {
        if (j == "discard") x = DiscardMsgType::DISCARD;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const DiscardMsgType & x) {
        switch (x) {
            case DiscardMsgType::DISCARD: j = "discard"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, GameErrorType & x) {
        if (j == "error") x = GameErrorType::ERROR;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const GameErrorType & x) {
        switch (x) {
            case GameErrorType::ERROR: j = "error"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, RedirectType & x) {
        if (j == "redirect") x = RedirectType::REDIRECT;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const RedirectType & x) {
        switch (x) {
            case RedirectType::REDIRECT: j = "redirect"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, GameStateType & x) {
        if (j == "game_state") x = GameStateType::GAME_STATE;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const GameStateType & x) {
        switch (x) {
            case GameStateType::GAME_STATE: j = "game_state"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, DealCardsMsgType & x) {
        if (j == "deal") x = DealCardsMsgType::DEAL;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const DealCardsMsgType & x) {
        switch (x) {
            case DealCardsMsgType::DEAL: j = "deal"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, WelcomeMsgType & x) {
        if (j == "welcome") x = WelcomeMsgType::WELCOME;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const WelcomeMsgType & x) {
        switch (x) {
            case WelcomeMsgType::WELCOME: j = "welcome"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, UndoPremoveMsgType & x) {
        if (j == "undo_premove") x = UndoPremoveMsgType::UNDO_PREMOVE;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const UndoPremoveMsgType & x) {
        switch (x) {
            case UndoPremoveMsgType::UNDO_PREMOVE: j = "undo_premove"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, PremoveMsgType & x) {
        if (j == "premove") x = PremoveMsgType::PREMOVE;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const PremoveMsgType & x) {
        switch (x) {
            case PremoveMsgType::PREMOVE: j = "premove"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, RestartMsgType & x) {
        if (j == "restart") x = RestartMsgType::RESTART;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const RestartMsgType & x) {
        switch (x) {
            case RestartMsgType::RESTART: j = "restart"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, JoinMsgType & x) {
        if (j == "join") x = JoinMsgType::JOIN;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const JoinMsgType & x) {
        switch (x) {
            case JoinMsgType::JOIN: j = "join"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, DisconnectMsgType & x) {
        if (j == "disconnect") x = DisconnectMsgType::DISCONNECT;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const DisconnectMsgType & x) {
        switch (x) {
            case DisconnectMsgType::DISCONNECT: j = "disconnect"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, ReconnectMsgType & x) {
        if (j == "reconnect") x = ReconnectMsgType::RECONNECT;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const ReconnectMsgType & x) {
        switch (x) {
            case ReconnectMsgType::RECONNECT: j = "reconnect"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, UpdateNameMsgType & x) {
        if (j == "update_name") x = UpdateNameMsgType::UPDATE_NAME;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const UpdateNameMsgType & x) {
        switch (x) {
            case UpdateNameMsgType::UPDATE_NAME: j = "update_name"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, PlayerUpdateMsgType & x) {
        if (j == "update_player") x = PlayerUpdateMsgType::UPDATE_PLAYER;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const PlayerUpdateMsgType & x) {
        switch (x) {
            case PlayerUpdateMsgType::UPDATE_PLAYER: j = "update_player"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }

    inline void from_json(const json & j, UpdateMsgType & x) {
        if (j == "update") x = UpdateMsgType::UPDATE;
        else { throw std::runtime_error("Input JSON does not conform to schema!"); }
    }

    inline void to_json(json & j, const UpdateMsgType & x) {
        switch (x) {
            case UpdateMsgType::UPDATE: j = "update"; break;
            default: throw std::runtime_error("This should not happen");
        }
    }
}
namespace nlohmann {
    inline void adl_serializer<std::variant<API::RichTextChunk, std::string>>::from_json(const json & j, std::variant<API::RichTextChunk, std::string> & x) {
        if (j.is_string())
            x = j.get<std::string>();
        else if (j.is_object())
            x = j.get<API::RichTextChunk>();
        else throw std::runtime_error("Could not deserialise!");
    }

    inline void adl_serializer<std::variant<API::RichTextChunk, std::string>>::to_json(json & j, const std::variant<API::RichTextChunk, std::string> & x) {
        switch (x.index()) {
            case 0:
                j = std::get<API::RichTextChunk>(x);
                break;
            case 1:
                j = std::get<std::string>(x);
                break;
            default: throw std::runtime_error("Input JSON does not conform to schema!");
        }
    }
}
namespace API {
std::string CardElement::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void CardElement::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string Card::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void Card::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string ClientMsg::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void ClientMsg::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string CookieMsg::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void CookieMsg::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string DealCardsMsg::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void DealCardsMsg::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string DiscardMsg::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void DiscardMsg::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string DisconnectMsg::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void DisconnectMsg::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string GameError::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void GameError::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string GameState::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void GameState::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string JoinMsg::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void JoinMsg::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string LastCardMsg::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void LastCardMsg::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string LayDownMsg::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void LayDownMsg::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string OrderMsg::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void OrderMsg::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string PassMsg::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void PassMsg::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string PlayCardMsg::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void PlayCardMsg::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string Player::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void Player::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string PlayerUpdateMsg::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void PlayerUpdateMsg::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string PlayJajaDingDongMsg::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void PlayJajaDingDongMsg::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string PremoveMsg::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void PremoveMsg::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string ReconnectMsg::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void ReconnectMsg::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string Redirect::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void Redirect::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string RestartMsg::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void RestartMsg::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string RichTextChunk::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void RichTextChunk::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string RichTextMsg::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void RichTextMsg::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string ServerDiscardMsg::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void ServerDiscardMsg::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string ServerMsg::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void ServerMsg::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string ServerPlayer::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void ServerPlayer::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string TableTalkMsg::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void TableTalkMsg::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string TaggedCard::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void TaggedCard::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string UndoPremoveMsg::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void UndoPremoveMsg::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string UpdateMsg::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void UpdateMsg::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string UpdateNameMsg::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void UpdateNameMsg::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string WelcomeMsg::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void WelcomeMsg::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string WinGameMsg::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void WinGameMsg::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string WinHandMsg::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void WinHandMsg::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
std::string WinTrickMsg::toString() const {
json j;
to_json(j, *this);
return j.dump();
}
void WinTrickMsg::fromString(const std::string &s) {
auto j = json::parse(s);
from_json(j, *this);
}
}
