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
