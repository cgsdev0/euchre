extends Node3D

var suit = "spades":
	get:
		return suit
	set(value):
		suit = value
		update_texture()
		
var rank = "ace" :
	get:
		return rank
	set(value):
		rank = value
		update_texture()
		
func update_texture():
	$Front.texture = Card.get_texture(suit, rank)
	
# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.

var v
var last

var tcard = preload("res://scenes/throwable_card.tscn")
func throw():
	var new_card = tcard.instantiate()
	new_card.suit = suit
	new_card.rank = rank
	get_owner().add_child(new_card)
	hide()
	new_card.global_transform = global_transform
	new_card.top_level = true
	new_card.show()
	new_card.freeze = false
	new_card.linear_velocity = v
	new_card.collision_layer = 1
	new_card.collision_mask = 1
	new_card.add_to_group("current_trick")
	
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _physics_process(delta: float) -> void:
	if last == null:
		last = global_position
		return
	v = (global_position - last) * 2.0
