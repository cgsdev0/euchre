extends VBoxContainer


var cache = {}

var ranks = ["nine", "ten", "jack", "queen", "king", "ace"]
var indices = [9, 10, 11, 12, 13, 1]
func _ready() -> void:
	for i in indices.size():
		var idx = indices[i]
		for suit in ["Hearts", "Diamonds", "Clubs", "Spades"]:
			var key = ranks[i] + "_" + suit.to_lower()
			cache[key] = load("res://debug_ui/cards/"+suit+" "+str(idx)+".png")


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	for i in get_child_count():
		var child = get_child(i)
		if i >= Client.state.trick.size():
			child.hide()
			continue
		child.show()
		var card = Client.state.trick[i].card
		child.texture = cache[card.rank + "_" + card.suit]
