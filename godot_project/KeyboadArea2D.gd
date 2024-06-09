extends Area2D

var is_pressed:bool = false
var note:int

func _ready()->void:
	input_event.connect(_on_input_event)
	mouse_exited .connect(_on_mouse_exited)

@warning_ignore("unused_parameter")
func _on_mouse_exited()->void:
	if is_pressed == true:
		is_pressed = false
		$"..".keyb_note_off(note)

@warning_ignore("unused_parameter")
func _on_input_event(viewport:Node, event:InputEvent , shape_idx:int)->void:
	if event is InputEventMouseButton and event.button_index == MOUSE_BUTTON_LEFT:
		var instance_name = get_name()
		if instance_name.begins_with("KEY"):
			if 	event.pressed:
				is_pressed = true
				note = int(instance_name.replace("KEY", ""))
				$"..".keyb_note_on(note)
			else:
				is_pressed = false
				$"..".keyb_note_off(note)


