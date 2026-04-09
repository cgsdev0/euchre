extends Node

@export var prod = true
@export var room = "pizzaA"

func cookie_hack():
	return OS.get_processor_name()
	
func get_url():
	if prod:
		return "wss://euchre.lol/ws/room/"
	else:
		return "ws://localhost:3001/ws/room/"

# Our WebSocketClient instance.
var socket = WebSocketPeer.new()

var reconnecting = false
var cooling_down = false
var initial_state = {
  "id": 0,
  "played_cards": [],
  "trump": "hearts",
  "your_cards": [],
  "trick": [],
  "scores": [0, 0],
  "players": [],
  "rich_chat_log": [],
  "turn": 0,
  "dealer": 0,
  "phase": "lobby",
  "private_session": false,
}

var state = initial_state

var queue = []

func send(data: Variant):
	socket.send_text(JSON.stringify(data))

func _ready():
	self.resume.connect(on_resume)
	connect_ws()
	
func on_resume():
	cooling_down = false
	
func connect_ws():
	var actual_url = get_url() + room
	# Initiate connection to the given URL.
	var err = socket.connect_to_url(actual_url)
	if err == OK:
		print("Connecting to %s..." % actual_url)
	else:
		push_error("Unable to connect.")
		set_process(false)

func intify_array(data: Array):
	for i in data.size():
		if typeof(data[i]) == TYPE_DICTIONARY:
			intify_dict(data[i])
		elif typeof(data[i]) == TYPE_ARRAY:
			intify_array(data[i])
		elif typeof(data[i]) == TYPE_FLOAT:
			data[i] = int(data[i])
			
func intify_dict(data: Dictionary):
	for key in data:
		if typeof(data[key]) == TYPE_DICTIONARY:
			intify_dict(data[key])
		elif typeof(data[key]) == TYPE_ARRAY:
			intify_array(data[key])
		elif typeof(data[key]) == TYPE_FLOAT:
			data[key] = int(data[key])

func merge(a, b):
	for key in b:
		a[key] = b[key]

signal stand_down(id)
signal play_card(id, card)
signal pick_up(id, card)
signal last_card(cards)
signal discard(id, card)
signal win_trick(id)
signal update_player(id)
signal welcome
signal resume
signal deal
signal think(id)
signal order

func apply_queued_action():
	if queue.is_empty() || cooling_down:
		return
	var action = queue.pop_front()
	print("Applying ", action.type)
	match action.type:
		"join":
			state.players.push_back({ "tricks": 0, "card_count": 0, "connected": true })
		"disconnect":
			state.players[action.id].connected = false
		"reconnect":
			state.players[action.id].connected = true
		"welcome":
			merge(state, action)
			self.welcome.emit()
			print(state)
		"update_player":
			if 'name' in action:
				state.players[action.id]['name'] = action.name
			print(action)
			self.update_player.emit(action.id)
		"update":
			merge(state, action)
			# detect voting changes
			match state.phase:
				"vote_round1":
					cooling_down = true
					think.emit(state.turn)
				"vote_round2":
					cooling_down = true
					think.emit(state.turn)
		"deal":
			for player in state.players:
				player.card_count = 5
			merge(state, action)
			cooling_down = true
			self.deal.emit()
			print(state)
		"order":
			order.emit()
			if "alone" in action && action.alone:
				var partner = (action.id + 2) % 4
				state.players[partner].sitting_out = true
				stand_down.emit(partner)
			if state.phase == "vote_round1":
				pick_up.emit(state.dealer, state.top_card)
			if state.dealer == state.id and state.phase == "vote_round1":
				state.your_cards.push_back(state.top_card)
			elif state.phase == "vote_round1":
				state.players[state.dealer].card_count += 1
		"discarded":
			cooling_down = true
			if state.dealer != state.id:
				discard.emit(state.dealer, null)
				state.players[action.id].card_count -= 1
		"restart":
			state.scores = [0, 0]
		"win_trick":
			cooling_down = true
			win_trick.emit(action.id)
			state.trick = []
			state.players[action.id].tricks += 1
		"win_hand":
			for n in get_tree().get_nodes_in_group("current_hand"):
				n.queue_free()
	
			state.played_cards = []
			for i in state.players.size():
				var player = state.players[i]
				player.sitting_out = false
				player.tricks = 0
		"play_card":
			cooling_down = true
			play_card.emit(action.id, action.card)
			state.players[action.id].card_count -= 1
			state.trick.push_back({"card": action.card, "id": action.id})
			state.played_cards.push_back({"card": action.card, "id": action.id})
		"last_card":
			cooling_down = true
			last_card.emit(action.cards)
			state.trick = action.cards
	
func _process(_delta):
	apply_queued_action()
	# Call this in `_process()` or `_physics_process()`.
	# Data transfer and state updates will only happen when calling this function.
	socket.poll()

	# get_ready_state() tells you what state the socket is in.
	var state = socket.get_ready_state()

	# `WebSocketPeer.STATE_OPEN` means the socket is connected and ready
	# to send and receive data.
	if state == WebSocketPeer.STATE_OPEN:
		while socket.get_available_packet_count():
			var packet = socket.get_packet()
			if socket.was_string_packet():
				var packet_text = packet.get_string_from_utf8()
				var data = JSON.parse_string(packet_text)
				intify_dict(data)
				match data.type:
					"error":
						match data.error:
							"cookie":
								var cookie = { "type": "cookie", "session": cookie_hack() }
								socket.send_text(JSON.stringify(cookie))
								var update_name = { "type": "update_name", "name": cookie_hack() }
								socket.send_text(JSON.stringify(update_name))
							_:
								printerr(data.error)
					"redirect":
						socket.close()
						room = data.room
						connect_ws()
					_:
						print("Received ", data.type)
						queue.push_back(data)
	# `WebSocketPeer.STATE_CLOSING` means the socket is closing.
	# It is important to keep polling for a clean close.
	elif state == WebSocketPeer.STATE_CLOSING:
		pass

	# `WebSocketPeer.STATE_CLOSED` means the connection has fully closed.
	# It is now safe to stop polling.
	elif state == WebSocketPeer.STATE_CLOSED:
		var code = socket.get_close_code()
		if !reconnecting:
			reconnecting = true
			print("WebSocket closed with code: %d. Clean: %s" % [code, code != -1])
			await get_tree().create_timer(3.0).timeout
			connect_ws()
			reconnecting = false
