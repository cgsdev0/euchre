extends Node3D

@onready var marker = $Marker3D
@onready var cards = $Cards

static var card = preload("res://scenes/card.tscn")

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	Client.play_card.connect(on_play_card)
	Client.last_card.connect(on_last_card)
	Client.deal.connect(on_deal)
	Client.welcome.connect(on_welcome)
	Client.pick_up.connect(on_pick_up)
	Client.discard.connect(on_discard)
	Client.win_trick.connect(on_win_trick)
var dummy = true

func on_win_trick(pid):
	if pid == get_index():
		var t = create_tween()
		t.set_parallel()
		t.set_trans(Tween.TRANS_QUAD)
		t.set_ease(Tween.EASE_OUT)
		for n in get_tree().get_nodes_in_group("current_trick"):
			n.remove_from_group("current_trick")
			n.add_to_group("current_hand")
			n.freeze = true
			t.tween_property(n, "global_position", get_tricks_marker().global_position, 0.8)
			t.tween_property(n, "global_rotation", get_tricks_marker().global_rotation, 0.8)
		await t.finished
		Client.resume.emit()
			
func get_tricks_marker():
	return $Tricks
	
func on_welcome():
	dummy = get_index() != Client.state.id
	if !dummy:
		get_viewport().get_camera_3d().global_transform = marker.global_transform
		get_viewport().get_camera_3d().global_basis = marker.global_basis
		for i in Client.state.your_cards.size():
			if i >= cards.get_child_count():
				cards.add_child(card.instantiate())
				relayout()
			var c = Client.state.your_cards[i]
			var ci = cards.get_child(i)
			ci.suit = c.suit
			ci.rank = c.rank
	else:
		if Client.state.players.size() <= get_index():
			return
		var card_count = Client.state.players[get_index()].card_count
		while card_count > cards.get_child_count():
			cards.add_child(card.instantiate())
		while card_count < cards.get_child_count():
			cards.get_child(0).queue_free()
		relayout()
		
func on_pick_up(id, c):
	if id != get_index():
		return
	var ci = card.instantiate()
	cards.add_child(ci)
	ci.suit = c.suit
	ci.rank = c.rank
	relayout()
		
func on_deal():
	if !dummy:
		for i in Client.state.your_cards.size():
			if i >= cards.get_child_count():
				cards.add_child(card.instantiate())
				relayout()
			var c = Client.state.your_cards[i]
			var ci = cards.get_child(i)
			ci.suit = c.suit
			ci.rank = c.rank
			await get_tree().create_timer(0.2).timeout
		Client.resume.emit()
	else:
		while cards.get_child_count() < 5:
			cards.add_child(card.instantiate())
			relayout()

func on_last_card(cards):
	for i in cards.size():
		if cards[i].id == get_index():
			if !dummy:
				pending = cards[i].card
			%FakeCard.suit = cards[i].card.suit
			%FakeCard.rank = cards[i].card.rank
			$AnimationPlayer2.play("play_card")
			if i == 0:
				await $AnimationPlayer2.animation_finished
				Client.resume.emit()

var pending = null

func on_discard(i, card):
	if i == get_index():
		if !dummy:
			pending = card
		$AnimationPlayer2.play("play_card")
		await $AnimationPlayer2.animation_finished
		Client.resume.emit()
		
func on_play_card(i, card):
	if i == get_index():
		if !dummy:
			pending = card
		%FakeCard.suit = card.suit
		%FakeCard.rank = card.rank
		$AnimationPlayer2.play("play_card")
		await $AnimationPlayer2.animation_finished
		Client.resume.emit()

func hide_card():
	if dummy:
		var ci = cards.get_child(0)
		ci.queue_free()
		await RenderingServer.frame_post_draw
		relayout()
	else:
		if !pending:
			return
		for ci in cards.get_children():
			if ci.suit == pending.suit && ci.rank == pending.rank:
				ci.queue_free()
				await RenderingServer.frame_post_draw
				relayout()

@export var fanout = 0.08
@export var arch = 0.04
@export var fan_angle = 15.0

func relayout():
	var m = cards.get_child_count()
	var even = m % 2 == 0
	for i in m:
		var ci = cards.get_child(i)
		var pos = Vector3.ZERO
		pos.x = -(i - m / 2.0 + 0.5) * (fanout)
		pos.y = -abs((i - m / 2.0 + 0.5) * (arch))
		if i == m / 2 and !even:
			pos.y -= 0.02
		pos.z = -i * 0.01
		var rot = (i - m / 2.0 + 0.5) * (fan_angle)
		ci.apply_transform(pos, rot)
