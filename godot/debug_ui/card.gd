extends TextureRect

signal pressed

func _ready():
	gui_input.connect(_on_card_gui_input)
	
func _on_card_gui_input(event: InputEvent) -> void:
	if event is InputEventMouseButton && event.pressed:
		pressed.emit()
