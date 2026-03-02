extends HBoxContainer

var display_name = "Name"
var cards = 5
var connected = false
var dealer = false
var tricks = 0
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


func _process(delta: float) -> void:
	if dealer:
		$Name.text = "[D] " + display_name
	else:
		$Name.text = "      " + display_name
	$Cards.text = str(cards)
	$Connected.text = str(connected)
	$Tricks.text = str(tricks)
