extends Node


var cache = {}

var ranks = ["nine", "ten", "jack", "queen", "king", "ace"]
var indices = [9, 10, 11, 12, 13, 1]

func _ready() -> void:
	for i in indices.size():
		var idx = indices[i]
		for suit in ["Hearts", "Diamonds", "Clubs", "Spades"]:
			var key = ranks[i] + "_" + suit.to_lower()
			cache[key] = load("res://debug_ui/cards/"+suit+" "+str(idx)+".png")

func get_texture(suit, rank):
	return cache[rank + "_" + suit]
