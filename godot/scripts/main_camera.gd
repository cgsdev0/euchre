extends Camera3D


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass

func _input(event):
	if event is InputEventMouseButton:
		if event.pressed && event.button_index == 1:
			var space_state = get_world_3d().direct_space_state
			var mpos_2d = get_viewport().get_mouse_position()
			var raycast_origin = project_ray_origin(mpos_2d)
			var raycast_end = project_position(mpos_2d, 3)
			var query = PhysicsRayQueryParameters3D.create(raycast_origin, raycast_end)
			query.collide_with_areas = true
			var intersect = space_state.intersect_ray(query)
			print(intersect)
			if "collider" in intersect:
				var p = intersect.collider
				if is_instance_valid(p) && p.has_method("on_press"):
					p.on_press()
