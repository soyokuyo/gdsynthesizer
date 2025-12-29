extends Area2D

func _ready()->void:
	input_event.connect(_on_input_event)

@warning_ignore("unused_parameter")
func _on_input_event(viewport:Node, event:InputEvent, shape_idx:int)->void:
	if event is InputEventMouseButton:
		if event.button_index == MOUSE_BUTTON_RIGHT and event.pressed:
			# Right click: toggle piano roll visibility
			$"..".toggle_piano_roll()

