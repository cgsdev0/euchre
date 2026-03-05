extends VBoxContainer


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	for i in get_child_count():
		var child = get_child(i)
		if i >= Client.state.players.size():
			child.hide()
			continue
		child.show()
		if i == Client.state.turn:
			child.modulate = Color.YELLOW
		else:
			child.modulate = Color.WHITE
		var info = Client.state.players[i]
		child.display_name = info.get("name", "Player " + str(i))
		child.dealer = Client.state.dealer == i
		child.cards = info.card_count
		child.connected = info.connected
		child.tricks = info.tricks
