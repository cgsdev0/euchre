class_name PlayingCard extends Area3D

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

var tween
func apply_transform(pos, rot):
	if tween:
		tween.kill()
	tween = create_tween()
	tween.set_parallel()
	tween.set_ease(Tween.EASE_OUT)
	tween.set_trans(Tween.TRANS_CUBIC)
	tween.tween_property(self, "position", pos, 0.5)
	tween.tween_property(self, "rotation_degrees", Vector3(0.0, 0.0, rot), 0.5)
	
func cards_equal(a, b):
	return a.suit == b.suit and a.rank == b.rank

func as_card():
	return {"rank": rank, "suit": suit}
	
func on_press():
	var card = self.as_card()
	var i = get_index()
	match Client.state.phase:
		"discarding":
			if Client.state.dealer == Client.state.id and \
				not cards_equal(card, Client.state.top_card) and i < 5:
				Client.state.phase = "playing"
				Client.send({"type": "discard", "card": card})
				Client.discard.emit(Client.state.id, card)
				Client.state.your_cards.remove_at(i)
		"playing":
			if Client.state.turn == Client.state.id:
				Client.state.turn += 1
				Client.send({"type": "play_card", "card": card})
				Client.state.your_cards.remove_at(i)
			else:
				Client.send({"type": "premove", "card": card})
