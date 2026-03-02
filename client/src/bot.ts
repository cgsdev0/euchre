import type { ServerMsg, WelcomeMsg } from "types/api.ts";
import { WebSocket } from "ws";

const room = process.argv[2];
const bots = Number.parseInt(process.argv[3]);

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
const startBot = (room: string, cookie: string) => {
  let state: State = JSON.parse(JSON.stringify(initial_state)) as State;
  const setupWs = () => {
    const ws = new WebSocket(`ws://localhost:3001/ws/room/${room}`, {
      headers: { Cookie: `_session=${cookie}` },
    });

    ws.addEventListener("open", () => {
      console.log("opened", room);
    });

    ws.addEventListener("error", () => {
      console.log("error!");
    });
    return ws;
  };

  let ws = setupWs();

  const messageHandler = (e: WebSocket.MessageEvent) => {
    try {
      const data = JSON.parse(e.data.toString()) as string | ServerMsg;
      if (typeof data === "string") {
        console.error(data);
        return;
      }
      console.log("[message]", data.type);
      switch (data.type) {
        case "welcome":
          state = { ...state, ...data };
          if (state.players.length === 4) {
            ws.send(JSON.stringify({ type: "restart" }));
          }
          console.log(state);
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
    console.log("closed, reopening...");
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
  startBot(room, `bot${i}`);
}
