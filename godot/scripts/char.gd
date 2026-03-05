extends Node3D


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	Client.play_card.connect(on_play_card)
	Client.deal.connect(on_deal)

func on_deal():
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
	
func on_play_card(i, card):
	if i == get_index():
		$AnimationPlayer2.play("play_card")
		await $AnimationPlayer2.animation_finished
		Client.resume.emit()

var card_count = 5

func hide_card(card):
	card_count -= 1
	
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	for i in $Cards.get_child_count():
		$Cards.get_child(i).visible = i < card_count
