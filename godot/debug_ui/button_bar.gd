extends HBoxContainer

@export var phase = "vote_round1"

func _ready():
	for child in get_children():
		child.pressed.connect(on_press.bind(child.name))
		
func on_press(s: String):
	match s:
		"Pass":
			%Client.send({ "type": "pass" })
		"Order":
			%Client.send({ "type": "order", "alone": %Alone.button_pressed })
		"Start":
			%Client.send({ "type": "restart" })
		"PickHearts":
			%Client.send({ "type": "order", "suit": "hearts" })
		"PickDiamonds":
			%Client.send({ "type": "order", "suit": "diamonds" })
		"PickClubs":
			%Client.send({ "type": "order", "suit": "clubs" })
		"PickSpades":
			%Client.send({ "type": "order", "suit": "spades" })
			
func _process(delta: float) -> void:
	if phase != "lobby" and phase != "ended" and %Client.state.turn != %Client.state.id:
		hide()
		return
	visible = %Client.state.phase == phase
