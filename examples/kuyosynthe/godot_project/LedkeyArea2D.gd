extends Area2D

func _ready()->void:
	input_event.connect(_on_input_event)

@warning_ignore("unused_parameter")
func _on_input_event(viewport:Node, event:InputEvent , shape_idx:int)->void:
	if event is InputEventMouseButton and event.button_index == MOUSE_BUTTON_LEFT:
		var instance_name = get_name()
		if instance_name.begins_with("LED"):
			if 	event.pressed:
				Globalv.program = int(instance_name.replace("LED", ""))
				$"../GDSynthesizer".program_change()
				Globalv.is_keyboard_clear = true
				if Globalv.is_percussion_prog_select:
					Globalv.is_percussion_prog_select = false
					Globalv.percussion_prog = Globalv.program
					$"../GDSynthesizer".modify_percussion_params()
					$"..".all_led_off()
					var k:int = int(float(Globalv.percussion_prog)/128.0)
					var j:int = int(float(Globalv.percussion_prog%128)/8.0)
					var i:int = int(Globalv.percussion_prog%8)
					var pled_position:Vector2 = Vector2(
						(5+2)*(j*(8+1)+i)+12,
						(5+12)*k+Globalv.pled_pos_y+10+7)
					$"../PLed2".set("position", pled_position)
					$"../PLed2".color = Color(0.0, 1.0, 0.0, 1)
					
