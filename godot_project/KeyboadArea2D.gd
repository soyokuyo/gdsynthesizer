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
			var current_note = int(instance_name.replace("KEY", ""))
			var note_mod = current_note % 12
			
			# Black keys are: C# (1), D# (3), F# (6), G# (8), A# (10)
			var is_black_key = note_mod == 1 or note_mod == 3 or note_mod == 6 or note_mod == 8 or note_mod == 10
			
			if event.pressed:
				if is_black_key:
					# Black key: process normally
					is_pressed = true
					note = current_note
					$"..".keyb_note_on(note)
				else:
					# White key: check if a black key exists at the click position
					# Get click position in global coordinates
					var click_pos = event.global_position
					# Convert to local coordinates relative to parent
					var local_pos = $"..".to_local(click_pos)
					
					# Check if there's a black key that overlaps with this position
					# Black keys are positioned at pos_x-7, pos_y+50 with size 14x30
					# White keys are positioned at pos_x, pos_y+50 with size 14x60
					# Overlap area: black key bottom is at pos_y+50+30 = pos_y+80
					# White key top is at pos_y+50, so overlap is from pos_y+50 to pos_y+80
					
					# Check if click is in the overlap area (y between pos_y+50 and pos_y+80)
					var pos_y = 455  # Base position from FrontCase.gd
					if local_pos.y >= pos_y + 50 and local_pos.y <= pos_y + 80:
						# Check if there's a black key at this x position
						var black_key_note = -1
						if note_mod == 0:  # C -> C# (1)
							black_key_note = current_note + 1
						elif note_mod == 2:  # D -> D# (3)
							black_key_note = current_note + 1
						elif note_mod == 4:  # E -> check D# (3)
							black_key_note = current_note - 1
						elif note_mod == 5:  # F -> F# (6)
							black_key_note = current_note + 1
						elif note_mod == 7:  # G -> G# (8)
							black_key_note = current_note + 1
						elif note_mod == 9:  # A -> A# (10)
							black_key_note = current_note + 1
						elif note_mod == 11:  # B -> check A# (10)
							black_key_note = current_note - 1
						
						# If black key exists, skip white key processing
						if black_key_note >= 0:
							if black_key_note >= Globalv.most_left_key_num and \
							   black_key_note < Globalv.most_left_key_num + Globalv.num_keyboard_key:
								var black_key_node = $"..".get_node_or_null("KEY" + str(black_key_note))
								if black_key_node != null:
									# Black key exists at this position, skip white key
									return
					
					# Process white key normally
					is_pressed = true
					note = current_note
					$"..".keyb_note_on(note)
			else:
				is_pressed = false
				note = current_note
				$"..".keyb_note_off(note)


