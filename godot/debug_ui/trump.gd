extends Label


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	var phase = Client.state.phase
	if phase != "playing" and phase != "discarding":
		hide()
	else:
		show()
	text = "Trump: " + Client.state.trump.to_upper()
