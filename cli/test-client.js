#!/usr/bin/env node
'use strict';

const readline = require('readline');
const crypto = require('crypto');
const http = require('http');
const WebSocket = require('ws');

// --- ANSI colors ---
const C = {
  reset: '\x1b[0m',
  bold: '\x1b[1m',
  dim: '\x1b[2m',
  red: '\x1b[31m',
  green: '\x1b[32m',
  yellow: '\x1b[33m',
  blue: '\x1b[34m',
  magenta: '\x1b[35m',
  cyan: '\x1b[36m',
  white: '\x1b[37m',
  gray: '\x1b[90m',
};

// --- Card display ---
const SUIT_SYMBOLS = { clubs: '\u2663', diamonds: '\u2666', hearts: '\u2665', spades: '\u2660' };
const SUIT_COLORS = { clubs: C.green, diamonds: C.blue, hearts: C.red, spades: C.white };
const RANK_SHORT = { nine: '9', ten: '10', jack: 'J', queen: 'Q', king: 'K', ace: 'A' };

function fmtCard(card) {
  if (!card) return '??';
  const s = SUIT_SYMBOLS[card.suit] || '?';
  const r = RANK_SHORT[card.rank] || card.rank;
  const color = SUIT_COLORS[card.suit] || '';
  const illegal = card.illegal ? C.dim : '';
  return `${illegal}${color}${r}${s}${C.reset}`;
}

function fmtCards(cards) {
  if (!cards || !cards.length) return '(none)';
  return cards.map(fmtCard).join(' ');
}

// --- State tracking ---
let gameState = null;
let myId = null;

function mergeState(msg) {
  if (!gameState) gameState = {};
  for (const key of ['dealer', 'phase', 'scores', 'trump', 'turn', 'players',
                      'played_cards', 'trick', 'top_card', 'your_cards', 'id',
                      'rich_chat_log', 'private_session']) {
    if (msg[key] !== undefined) gameState[key] = msg[key];
  }
  if (msg.type === 'welcome') myId = msg.id;
}

function statusLine() {
  if (!gameState) return '';
  const phase = gameState.phase || '?';
  const trump = gameState.trump ? `${SUIT_SYMBOLS[gameState.trump] || gameState.trump}` : '-';
  const scores = gameState.scores ? gameState.scores.join('-') : '?';
  const turn = gameState.turn !== undefined ? gameState.turn : '?';
  const dealer = gameState.dealer !== undefined ? gameState.dealer : '?';
  const hand = fmtCards(gameState.your_cards);
  const myTurn = turn === myId;
  const turnIndicator = myTurn ? `${C.bold}${C.green}>>> YOUR TURN <<<${C.reset}` : `turn:${turn}`;
  return `${C.gray}[${phase}] trump:${trump} scores:${scores} dealer:${dealer} ${turnIndicator} hand: ${hand}${C.reset}`;
}

function playerName(id) {
  if (!gameState || !gameState.players || !gameState.players[id]) return `Player ${id}`;
  return gameState.players[id].name || `Player ${id}`;
}

// --- Message formatting ---
const MSG_COLORS = {
  welcome: C.green,
  deal: C.green,
  update: C.cyan,
  join: C.green,
  disconnect: C.yellow,
  reconnect: C.green,
  pass: C.gray,
  order: C.magenta,
  play_card: C.yellow,
  chat: C.white,
  error: C.red,
  redirect: C.yellow,
  update_name: C.cyan,
  table_talk: C.gray,
  play_jaja_ding_dong: C.magenta,
};

function formatIncoming(msg) {
  const color = MSG_COLORS[msg.type] || C.white;
  const prefix = `${color}${C.bold}[${msg.type}]${C.reset}`;

  switch (msg.type) {
    case 'welcome':
      return `${prefix} You are player ${C.bold}${msg.id}${C.reset} | phase: ${msg.phase} | players: ${msg.players.map((p, i) => `${i}:${p.name}`).join(', ')}`;
    case 'deal':
      return `${prefix} top: ${fmtCard(msg.top_card)} | your cards: ${fmtCards(msg.your_cards)}`;
    case 'update':
      return `${prefix} phase:${msg.phase} trump:${msg.trump || '-'} turn:${msg.turn} dealer:${msg.dealer} scores:${msg.scores?.join('-')}`;
    case 'join':
      return `${prefix} ${playerName(msg.id)} (seat ${msg.id}) joined`;
    case 'disconnect':
      return `${prefix} ${playerName(msg.id)} disconnected`;
    case 'reconnect':
      return `${prefix} ${playerName(msg.id)} reconnected`;
    case 'pass':
      return `${prefix} ${playerName(msg.id)} passed`;
    case 'order':
      return `${prefix} ${playerName(msg.id)} ordered ${SUIT_SYMBOLS[msg.suit] || msg.suit}${msg.alone ? ' (going alone!)' : ''}`;
    case 'play_card':
      return `${prefix} ${playerName(msg.id)} played ${fmtCard(msg.card)}`;
    case 'update_name':
      return `${prefix} Player ${msg.id} is now "${msg.name}"`;
    case 'error':
      return `${prefix} ${C.red}${msg.error}${C.reset}`;
    case 'redirect':
      return `${prefix} Redirecting to room: ${msg.room}`;
    case 'chat':
      return `${prefix} ${formatRichText(msg.msg)}`;
    default:
      return `${prefix} ${JSON.stringify(msg)}`;
  }
}

function formatRichText(chunks) {
  if (!chunks) return '';
  return chunks.map(c => {
    if (typeof c === 'string') return c;
    if (c.type === 'rt_username') return `${C.bold}${c.text}${C.reset}`;
    return c.text || '';
  }).join('');
}

// --- Suit/rank parsing helpers ---
const SUIT_ALIASES = {
  c: 'clubs', clubs: 'clubs', club: 'clubs',
  d: 'diamonds', diamonds: 'diamonds', diamond: 'diamonds',
  h: 'hearts', hearts: 'hearts', heart: 'hearts',
  s: 'spades', spades: 'spades', spade: 'spades',
  '\u2663': 'clubs', '\u2666': 'diamonds', '\u2665': 'hearts', '\u2660': 'spades',
};

const RANK_ALIASES = {
  '9': 'nine', nine: 'nine', n: 'nine',
  '10': 'ten', ten: 'ten', t: 'ten',
  j: 'jack', jack: 'jack',
  q: 'queen', queen: 'queen',
  k: 'king', king: 'king',
  a: 'ace', ace: 'ace',
};

function parseSuit(s) {
  return SUIT_ALIASES[(s || '').toLowerCase()] || null;
}

function parseRank(r) {
  return RANK_ALIASES[(r || '').toLowerCase()] || null;
}

function parseCard(args) {
  // Accept: "ace hearts", "a h", "ah", "ace h"
  if (args.length >= 2) {
    const rank = parseRank(args[0]);
    const suit = parseSuit(args[1]);
    if (rank && suit) return { rank, suit };
    // Try reversed
    const rank2 = parseRank(args[1]);
    const suit2 = parseSuit(args[0]);
    if (rank2 && suit2) return { rank: rank2, suit: suit2 };
  }
  if (args.length === 1) {
    const s = args[0].toLowerCase();
    // Try "ah" = ace hearts, "9c" = nine clubs, "10d" = ten diamonds
    for (const [rk, rv] of Object.entries(RANK_ALIASES)) {
      if (s.startsWith(rk)) {
        const rest = s.slice(rk.length);
        const sv = parseSuit(rest);
        if (sv) return { rank: rv, suit: sv };
      }
    }
  }
  return null;
}

// --- Command handling ---
function handleCommand(line, ws) {
  const trimmed = line.trim();
  if (!trimmed) return;

  const parts = trimmed.split(/\s+/);
  const cmd = parts[0].toLowerCase();
  const args = parts.slice(1);

  switch (cmd) {
    case 'pass':
      send(ws, { type: 'pass' });
      break;

    case 'order': {
      const suit = parseSuit(args[0]);
      if (!suit) {
        log(`${C.red}Usage: order <suit> [alone]${C.reset}`);
        log(`  Suits: clubs/c, diamonds/d, hearts/h, spades/s`);
        return;
      }
      const alone = args.length > 1 && ['alone', 'true', '1', 'yes'].includes(args[1].toLowerCase());
      const msg = { type: 'order', suit };
      if (alone) msg.alone = true;
      send(ws, msg);
      break;
    }

    case 'play': {
      const card = parseCard(args);
      if (!card) {
        log(`${C.red}Usage: play <rank> <suit>  (e.g. play ace hearts, play ah, play a h)${C.reset}`);
        return;
      }
      send(ws, { type: 'play_card', card });
      break;
    }

    case 'discard': {
      const card = parseCard(args);
      if (!card) {
        log(`${C.red}Usage: discard <rank> <suit>  (e.g. discard 9 clubs, discard 9c)${C.reset}`);
        return;
      }
      send(ws, { type: 'discard', card });
      break;
    }

    case 'restart':
      send(ws, { type: 'restart' });
      break;

    case 'name': {
      const name = args.join(' ');
      if (!name) {
        log(`${C.red}Usage: name <your name>${C.reset}`);
        return;
      }
      send(ws, { type: 'update_name', name });
      break;
    }

    case 'talk':
    case 'tabletalk': {
      const idx = parseInt(args[0], 10);
      if (isNaN(idx)) {
        log(`${C.red}Usage: talk <index>${C.reset}`);
        return;
      }
      send(ws, { type: 'table_talk', table_talk: idx });
      break;
    }

    case 'jaja':
      send(ws, { type: 'play_jaja_ding_dong' });
      break;

    case 'raw': {
      const json = args.join(' ');
      try {
        const parsed = JSON.parse(json);
        send(ws, parsed);
      } catch (e) {
        log(`${C.red}Invalid JSON: ${e.message}${C.reset}`);
      }
      break;
    }

    case 'state':
      if (!gameState) {
        log(`${C.yellow}No game state yet${C.reset}`);
      } else {
        log(JSON.stringify(gameState, null, 2));
      }
      break;

    case 'hand':
      if (!gameState || !gameState.your_cards) {
        log(`${C.yellow}No cards yet${C.reset}`);
      } else {
        log(`Your cards: ${fmtCards(gameState.your_cards)}`);
      }
      break;

    case 'players': {
      if (!gameState || !gameState.players) {
        log(`${C.yellow}No player info yet${C.reset}`);
        return;
      }
      gameState.players.forEach((p, i) => {
        const me = i === myId ? ` ${C.green}(you)${C.reset}` : '';
        const conn = p.connected ? `${C.green}connected${C.reset}` : `${C.red}disconnected${C.reset}`;
        const team = i % 2 === 0 ? 'A' : 'B';
        log(`  ${i}: ${C.bold}${p.name}${C.reset}${me} [team ${team}] ${conn} cards:${p.card_count} tricks:${p.tricks}`);
      });
      break;
    }

    case 'help':
      printHelp();
      break;

    case 'quit':
    case 'exit':
    case 'q':
      log('Closing connection...');
      ws.close();
      process.exit(0);
      break;

    default:
      log(`${C.red}Unknown command: ${cmd}. Type 'help' for commands.${C.reset}`);
  }
}

function send(ws, obj) {
  const json = JSON.stringify(obj);
  log(`${C.dim}>>> ${json}${C.reset}`);
  ws.send(json);
}

function log(msg) {
  // Clear the readline prompt, print, then re-render prompt
  process.stdout.write('\r\x1b[K');
  console.log(msg);
  if (rl) rl.prompt(true);
}

function printHelp() {
  log(`
${C.bold}Commands:${C.reset}
  ${C.cyan}pass${C.reset}                    Pass during voting
  ${C.cyan}order <suit> [alone]${C.reset}    Order up trump (suits: c/d/h/s or full name)
  ${C.cyan}play <rank> <suit>${C.reset}      Play a card (e.g. play ah, play ace hearts)
  ${C.cyan}discard <rank> <suit>${C.reset}   Discard a card (dealer, after ordering)
  ${C.cyan}restart${C.reset}                 Start/restart the game
  ${C.cyan}name <name>${C.reset}             Set your display name
  ${C.cyan}talk <index>${C.reset}            Send table talk
  ${C.cyan}jaja${C.reset}                    Play jaja ding dong
  ${C.cyan}raw <json>${C.reset}              Send arbitrary JSON
  ${C.cyan}state${C.reset}                   Print full game state
  ${C.cyan}hand${C.reset}                    Print your cards
  ${C.cyan}players${C.reset}                 Print player info
  ${C.cyan}help${C.reset}                    Show this help
  ${C.cyan}quit${C.reset}                    Close connection

${C.bold}Card shorthand:${C.reset} rank + suit initial: ${fmtCard({rank:'ace',suit:'spades'})} = as, ${fmtCard({rank:'king',suit:'hearts'})} = kh, ${fmtCard({rank:'nine',suit:'clubs'})} = 9c
${C.bold}Suits:${C.reset} c(lubs), d(iamonds), h(earts), s(pades)
${C.bold}Ranks:${C.reset} 9, 10/t, j(ack), q(ueen), k(ing), a(ce)
`);
}

// --- HTTP helper ---
function httpGet(url, cookie) {
  return new Promise((resolve, reject) => {
    const opts = new URL(url);
    const headers = cookie ? { Cookie: cookie } : {};
    http.get({ hostname: opts.hostname, port: opts.port, path: opts.pathname + opts.search, headers }, (res) => {
      let data = '';
      res.on('data', chunk => data += chunk);
      res.on('end', () => {
        if (res.statusCode >= 400) reject(new Error(`HTTP ${res.statusCode}: ${data}`));
        else resolve(data);
      });
    }).on('error', reject);
  });
}

// --- Main ---
let rl = null;

async function main() {
  const hostArg = process.argv[2] || 'localhost:3001';
  let room = process.argv[3] || null;

  // Ensure host has protocol
  const host = hostArg.includes('://') ? hostArg.replace(/^https?/, 'http') : `http://${hostArg}`;
  const wsHost = host.replace(/^http/, 'ws');

  // Generate session
  const session = crypto.randomUUID();
  const cookie = `_session=${session}`;

  console.log(`${C.bold}Euchre Test Client${C.reset}`);
  console.log(`${C.gray}Server: ${host}${C.reset}`);
  console.log(`${C.gray}Session: ${session}${C.reset}`);

  // Create room if needed
  if (!room) {
    try {
      room = await httpGet(`${host}/create`, cookie);
      room = room.trim();
      console.log(`${C.green}Created room: ${C.bold}${room}${C.reset}`);
    } catch (e) {
      console.error(`${C.red}Failed to create room: ${e.message}${C.reset}`);
      process.exit(1);
    }
  } else {
    console.log(`${C.cyan}Joining room: ${C.bold}${room}${C.reset}`);
  }

  // Connect WebSocket
  const wsUrl = `${wsHost}/ws/room/${room}`;
  console.log(`${C.gray}Connecting to ${wsUrl}...${C.reset}`);

  const ws = new WebSocket(wsUrl, { headers: { Cookie: cookie } });

  ws.on('open', () => {
    console.log(`${C.green}${C.bold}Connected!${C.reset} Room: ${C.bold}${room}${C.reset}`);
    console.log(`${C.gray}Type 'help' for commands${C.reset}\n`);
    startRepl(ws);
  });

  ws.on('message', (data) => {
    try {
      const msg = JSON.parse(data.toString());
      // Merge state
      if (['welcome', 'update', 'deal'].includes(msg.type)) {
        mergeState(msg);
      }
      if (msg.type === 'join' && gameState && gameState.players && msg.id !== undefined) {
        if (!gameState.players[msg.id]) gameState.players[msg.id] = {};
        gameState.players[msg.id].name = msg.name;
        gameState.players[msg.id].connected = true;
      }
      if (msg.type === 'disconnect' && gameState && gameState.players && gameState.players[msg.id]) {
        gameState.players[msg.id].connected = false;
      }
      if (msg.type === 'reconnect' && gameState && gameState.players && gameState.players[msg.id]) {
        gameState.players[msg.id].connected = true;
      }
      if (msg.type === 'update_name' && gameState && gameState.players && gameState.players[msg.id]) {
        gameState.players[msg.id].name = msg.name;
      }
      if (msg.type === 'play_card' && gameState) {
        if (!gameState.trick) gameState.trick = [];
        gameState.trick.push(msg.card);
        // Remove from our hand if we played it
        if (msg.id === myId && gameState.your_cards) {
          gameState.your_cards = gameState.your_cards.filter(
            c => !(c.suit === msg.card.suit && c.rank === msg.card.rank)
          );
        }
      }

      // Print formatted message
      log(formatIncoming(msg));

      // Print status line after state-affecting messages
      if (['welcome', 'update', 'deal', 'order', 'pass', 'play_card'].includes(msg.type)) {
        const sl = statusLine();
        if (sl) log(sl);
      }
    } catch (e) {
      log(`${C.red}Failed to parse message: ${data}${C.reset}`);
    }
  });

  ws.on('close', (code, reason) => {
    console.log(`\n${C.yellow}Connection closed (code: ${code}${reason ? `, reason: ${reason}` : ''})${C.reset}`);
    process.exit(0);
  });

  ws.on('error', (err) => {
    console.error(`${C.red}WebSocket error: ${err.message}${C.reset}`);
    process.exit(1);
  });
}

function startRepl(ws) {
  rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
    prompt: `${C.bold}euchre>${C.reset} `,
  });

  rl.prompt();

  rl.on('line', (line) => {
    handleCommand(line, ws);
    rl.prompt();
  });

  rl.on('close', () => {
    ws.close();
    process.exit(0);
  });
}

main().catch(err => {
  console.error(`${C.red}Fatal: ${err.message}${C.reset}`);
  process.exit(1);
});
