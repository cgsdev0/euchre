extends HBoxContainer


var cache = {}

var ranks = ["nine", "ten", "jack", "queen", "king", "ace"]
var indices = [9, 10, 11, 12, 13, 1]
func _ready() -> void:
	for child in get_children():
		child.pressed.connect(on_press.bind(child.get_index()))
	for i in indices.size():
		var idx = indices[i]
		for suit in ["Hearts", "Diamonds", "Clubs", "Spades"]:
			var key = ranks[i] + "_" + suit.to_lower()
			cache[key] = load("res://debug_ui/cards/"+suit+" "+str(idx)+".png")

func cards_equal(a, b):
	return a.suit == b.suit and a.rank == b.rank

func on_press(i: int):
	match Client.state.phase:
		"discarding":
			if Client.state.dealer == Client.state.id and \
				not cards_equal(Client.state.your_cards[i], Client.state.top_card):
				Client.send({"type": "discard", "card": Client.state.your_cards[i]})
				Client.discard.emit(Client.state.id, Client.state.your_cards[i])
				Client.state.your_cards.remove_at(i)
		"playing":
			if Client.state.turn == Client.state.id:
				Client.send({"type": "play_card", "card": Client.state.your_cards[i]})
				Client.state.your_cards.remove_at(i)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	for i in get_child_count():
		var child = get_child(i)
		if i >= Client.state.your_cards.size():
			child.hide()
			continue
		child.show()
		var card = Client.state.your_cards[i]
		child.texture = cache[card.rank + "_" + card.suit]
