extends Label


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	if not "top_card" in %Client.state:
		visible = false
		return
	visible = true
	text = "Up Card: " + %Client.state.top_card.rank + " of " + %Client.state.top_card.suit
