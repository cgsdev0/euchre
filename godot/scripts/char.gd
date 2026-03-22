extends Node3D

@onready var marker = $Marker3D

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	Client.play_card.connect(on_play_card)
	Client.last_card.connect(on_last_card)
	Client.deal.connect(on_deal)
	Client.welcome.connect(on_welcome)

func on_welcome():
	if get_index() == Client.state.id:
		get_viewport().get_camera_3d().global_transform = marker.global_transform
		get_viewport().get_camera_3d().global_basis = marker.global_basis
		for i in Client.state.your_cards.size():
			var c = Client.state.your_cards[i]
			var card = $Cards.get_child(i)
			card.suit = c.suit
			card.rank = c.rank
		card_count = Client.state.your_cards.size()
	# card_count = Client.state.players[]
func on_deal():
	if get_index() == Client.state.id:
		for i in Client.state.your_cards.size():
			var c = Client.state.your_cards[i]
			var card = $Cards.get_child(i)
			card.suit = c.suit
			card.rank = c.rank
			
	await get_tree().create_timer(0.2).timeout
	card_count = 1
	await get_tree().create_timer(0.2).timeout
	card_count = 2
	await get_tree().create_timer(0.2).timeout
	card_count = 3
	await get_tree().create_timer(0.2).timeout
	card_count = 4
	await get_tree().create_timer(0.2).timeout
	card_count = 5
	Client.resume.emit()

func on_last_card(cards):
	for i in cards.size():
		if cards[i].id == get_index():
			$AnimationPlayer2.play("play_card")
			if i == 0:
				await $AnimationPlayer2.animation_finished
				Client.resume.emit()
			
func on_play_card(i, card):
	if i == get_index():
		$AnimationPlayer2.play("play_card")
		await $AnimationPlayer2.animation_finished
		Client.resume.emit()

var card_count = 5

func hide_card():
	card_count -= 1
	update_visibility()
	for i in Client.state.your_cards.size():
		var c = Client.state.your_cards[i]
		var card = $Cards.get_child(i)
		card.suit = c.suit
		card.rank = c.rank

func update_visibility():
	for i in $Cards.get_child_count():
		$Cards.get_child(i).visible = i < card_count
		
func _process(delta: float) -> void:
	update_visibility()
