extends Node

@export var websocket_url = "ws://localhost:3001/ws/room/"
@export var room = "asdf123"

# Our WebSocketClient instance.
var socket = WebSocketPeer.new()

var reconnecting = false
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

var timer
var cooling_down = false
func _ready():
	timer = Timer.new()
	timer.one_shot = true
	add_child(timer)
	timer.timeout.connect(on_timeout)
	connect_ws()

func on_timeout():
	cooling_down = false
	
func connect_ws():
	var actual_url = websocket_url + room
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
		
func apply_queued_action():
	if queue.is_empty() || cooling_down:
		return
	var action = queue.pop_front()
	if action.type != "update":
		cooling_down = true
		timer.start(0.5)
	print("Applying ", action.type)
	match action.type:
		"update_name":
			state.players[action.id].name = action.name
		"disconnect":
			state.players[action.id].connected = false
		"reconnect":
			state.players[action.id].connected = true
		"welcome":
			merge(state, action)
			print(state)
		"update":
			merge(state, action)
			print(state)
		"deal":
			for player in state.players:
				player.card_count = 5
			merge(state, action)
			print(state)
		"order":
			if state.dealer == state.id and state.phase == "vote_round1":
				state.your_cards.push_back(state.top_card)
			elif state.phase == "vote_round1":
				state.players[state.dealer].card_count += 1
		"discarded":
			if state.dealer != state.id:
				state.players[action.id].card_count -= 1
		"restart":
			state.scores = [0, 0]
		"win_trick":
			state.trick = []
			state.players[action.id].tricks += 1
		"win_hand":
			state.played_cards = []
			for player in state.players:
				player.sitting_out = false
				player.tricks = 0
		"play_card":
			state.players[action.id].card_count -= 1
			state.trick.push_back(action.card)
			state.played_cards.push_back(action.card)
	
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
								var cookie = { "type": "cookie", "session": "asdf" }
								socket.send_text(JSON.stringify(cookie))
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
