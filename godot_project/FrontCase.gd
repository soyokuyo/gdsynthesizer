extends Node2D

const bar_size_x:float = 340.0
const bar_pos_x:int = 410

func redraw_signal_allocation_bar()->void:
	var total:float = Globalv.total_gain
	var wave:float = total*(1.0-Globalv.noise_ratio)
	var wave1:float = wave*Globalv.base_vs_others_ratio
	var wave2:float = wave1+(wave-wave1)*Globalv.side1_vs_side2_ratio
	$"Bar_aloc_gray".size = Vector2(int(total*bar_size_x),5)
	$"Bar_aloc_red".size = Vector2(int(wave*bar_size_x),5)
	$"Bar_aloc_green".size = Vector2(int(wave2*bar_size_x),5)
	$"Bar_aloc_blue".size = Vector2(int(wave1*bar_size_x),5)


func alocation_bar_preparation()->void:
	const bar_pos_y:int = 395+25

	var white_back_bar:ColorRect = ColorRect.new()
	white_back_bar.name = str("Bar_aloc_white_back")
	white_back_bar.color = Color(1.0, 1.0, 1.0, 1)
	white_back_bar.visible = true
	white_back_bar.size = Vector2(int(bar_size_x)+2,7)
	white_back_bar.position = Vector2(bar_pos_x-1,bar_pos_y-1)
	add_child(white_back_bar)
	white_back_bar.owner = self

	var black_back_bar:ColorRect = ColorRect.new()
	black_back_bar.name = str("Bar_aloc_black_back")
	black_back_bar.color = Color(0.0, 0.0, 0.0, 1)
	black_back_bar.visible = true
	black_back_bar.size = Vector2(int(bar_size_x),5)
	black_back_bar.position = Vector2(bar_pos_x,bar_pos_y)
	add_child(black_back_bar)
	black_back_bar.owner = self

	var gray_bar:ColorRect = ColorRect.new()
	gray_bar.name = str("Bar_aloc_gray")
	gray_bar.color = Color(0.8, 0.8, 0.8, 1)
	gray_bar.visible = true
	gray_bar.size = Vector2(5,5)
	gray_bar.position = Vector2(bar_pos_x,bar_pos_y)
	add_child(gray_bar)
	gray_bar.owner = self

	var red_bar:ColorRect = ColorRect.new()
	red_bar.name = str("Bar_aloc_red")
	red_bar.color = Color(1.0, 0.0, 0.0, 1)
	red_bar.visible = true
	red_bar.size = Vector2(5,5)
	red_bar.position = Vector2(bar_pos_x,bar_pos_y)
	add_child(red_bar)
	red_bar.owner = self

	var green_bar:ColorRect = ColorRect.new()
	green_bar.name = str("Bar_aloc_green")
	green_bar.color = Color(0.0, 1.0, 0.0, 1)
	green_bar.visible = true
	green_bar.size = Vector2(5,5)
	green_bar.position = Vector2(bar_pos_x,bar_pos_y)
	add_child(green_bar)
	green_bar.owner = self

	var blue_bar:ColorRect = ColorRect.new()
	blue_bar.name = str("Bar_aloc_blue")
	blue_bar.color = Color(0.0, 0.0, 1.0, 1)
	blue_bar.visible = true
	blue_bar.size = Vector2(5,5)
	blue_bar.position = Vector2(bar_pos_x,bar_pos_y)
	add_child(blue_bar)
	blue_bar.owner = self


func levelbar_preparation()->void:
	const bar_pos_y:int = 404+25
	var green_back_bar:ColorRect = ColorRect.new()
	green_back_bar.name = str("Bar_green_back")
	green_back_bar.color = Color(0.0, 1.0, 0.0, 1)
	green_back_bar.visible = true
	green_back_bar.size = Vector2(int((1.0/1.5)*bar_size_x)+1,7)
	green_back_bar.position = Vector2(bar_pos_x-1,bar_pos_y-1)
	add_child(green_back_bar)
	green_back_bar.owner = self

	var red_back_bar:ColorRect = ColorRect.new()
	red_back_bar.name = str("Bar_red_back")
	red_back_bar.color = Color(1.0, 0.0, 0.0, 1)
	red_back_bar.visible = true
	red_back_bar.size = Vector2(int((0.5/1.5)*bar_size_x)+2,7)
	red_back_bar.position \
		= Vector2(bar_pos_x+int((1.0/1.5)*bar_size_x),bar_pos_y-1)
	add_child(red_back_bar)
	red_back_bar.owner = self

	var gray_bar:ColorRect = ColorRect.new()
	gray_bar.name = str("Bar_gray")
	gray_bar.color = Color(0.2, 0.2, 0.2, 1)
	gray_bar.visible = true
	gray_bar.size = Vector2(int(bar_size_x),5)
	gray_bar.position = Vector2(bar_pos_x,bar_pos_y)
	add_child(gray_bar)
	gray_bar.owner = self

	var brawn_bar:ColorRect = ColorRect.new()
	brawn_bar.name = str("Bar_brawn")
	brawn_bar.color = Color(0.5, 0.1, 0.1, 1)
	brawn_bar.visible = false
	brawn_bar.size = Vector2(5,5)
	brawn_bar.position = Vector2(bar_pos_x,bar_pos_y)
	add_child(brawn_bar)
	brawn_bar.owner = self

	var red_bar:ColorRect = ColorRect.new()
	red_bar.name = str("Bar_red")
	red_bar.color = Color(1.0, 0.0, 0.0, 1)
	red_bar.visible = false
	red_bar.size = Vector2(5,5)
	red_bar.position = Vector2(bar_pos_x,bar_pos_y)
	add_child(red_bar)
	red_bar.owner = self

	var green_bar:ColorRect = ColorRect.new()
	green_bar.name = str("Bar_green")
	green_bar.color = Color(0.0, 1.0, 0.0, 1)
	green_bar.visible = false
	green_bar.size = Vector2(5,5)
	green_bar.position = Vector2(bar_pos_x,bar_pos_y)
	add_child(green_bar)
	green_bar.owner = self

func led_preparation()->void:
	# LEDs for program indicator.
	var pled:ColorRect = ColorRect.new()
	pled.name = str("PLed")
	pled.size = Vector2(7,7)
	pled.color = Color(1.0, 0.0, 0.0, 1)
	pled.position = Vector2(
						(5+2)*(0*(8+1)+0)+12,
						(5+12)*0+Globalv.pled_pos_y+10)
	pled.visible = true
	add_child(pled)
	pled.owner = self

	var pled2:ColorRect = ColorRect.new()
	pled2.name = str("PLed2")
	pled2.size = Vector2(5,2)
	pled2.color = Color(0.2, 0.2, 0.0, 1)
	pled2.position = Vector2(
						(5+2)*(0*(8+1)+0)+12,
						(5+12)*0+Globalv.pled_pos_y+10+8)
	pled2.visible = true
	add_child(pled2)
	pled2.owner = self
	
	var ledkey:Array[Area2D] = []
	var led:Array[ColorRect] = []
	var collision:Array[CollisionShape2D] = []

	var index:int =0
	for k in range(2):
		for j in range(16):
			for i in range(8):
				ledkey.push_back(Area2D.new())
				led.push_back(ColorRect.new())
				collision.push_back(CollisionShape2D.new())
				ledkey[index].name = "LED"+str(index)
				led[index].name = "Led"
				collision[index].name = "LedCollision"
				led[index].size = Vector2(5,5)
				var shape:RectangleShape2D = RectangleShape2D.new()
				shape.size = Vector2(6,6)
				collision[index].set_shape(shape)
				led[index].color = Color(0.2, 0.2, 0.0, 1)
				var x:int = (5+2)*(j*(8+1)+i)+13
				var y:int = (5+12)*k+Globalv.pled_pos_y+11
				ledkey[index].position = Vector2(x , y)
				collision[index].position = Vector2(2, 2)
				led[index].visible = true
				led[index].set_mouse_filter(Control.MOUSE_FILTER_IGNORE)
				ledkey[index].set_script(load("res://LedkeyArea2D.gd"))

				add_child(ledkey[index])
				ledkey[index].owner = self
				get_node("LED"+str(index)).add_child(led[index])
				led[index].owner = ledkey[index]
				get_node("LED"+str(index)).add_child(collision[index])
				collision[index].owner = ledkey[index]

				index += 1

	var picts:Array[Label] = []
	index = 0
	for k in range(2):
		for j in range(16):
			picts.push_back(Label.new())
			picts[index].name = "Pict"+str(index)
			picts[index].size = Vector2(50,22)
			picts[index].add_theme_font_size_override("font_size", 8)
			picts[index].position = Vector2(
										(5+2)*(j*(8+1))+12,
										(5+13)*k+Globalv.pled_pos_y)
			picts[index].text = str(k*128+j*8+1)
			picts[index].visible = true
			add_child(picts[index])
			picts[index].owner = self
			index += 1


func _init()->void:
	levelbar_preparation()
	alocation_bar_preparation()
	led_preparation()
	keyboard_preparation()
	piano_roll_background_preparation()
	# Globalv.is_percussionの初期値を設定（_process()で初回更新されるように）
	last_is_percussion = Globalv.is_percussion

var keyb = []
var is_sift = false
var piano_roll_background: ColorRect = null


func keyboard_preparation()->void:
	# Keyboard.
	keyb.resize(24)
	keyb.fill(false)
	var keyboard:Array[Area2D] = []
	var keyoff:Array[ColorRect] = []
	var keyon:Array[ColorRect] = []
	var keyled:Array[ColorRect] = []
	var keyled2:Array[ColorRect] = []
	var collision:Array[CollisionShape2D] = []
	var index:int = Globalv.most_left_key_num
	var pos_x:int = 10
	var pos_y:int = 505
	for i in range(Globalv.num_keyboard_key):
		keyboard.push_back(Area2D.new())
		keyoff.push_back(ColorRect.new())
		keyon.push_back(ColorRect.new())
		keyled.push_back(ColorRect.new())
		keyled2.push_back(ColorRect.new())
		collision.push_back(CollisionShape2D.new())
		
		keyboard[i].name = "KEY"+str(index)
		keyoff[i].name = "keyoff"
		keyon[i].name = "keyon"
		keyled[i].name = "keyled"
		keyled2[i].name = "keyled2"
		collision[i].name = "keyCollision"
		keyled[i].size = Vector2(14,3)
		keyled2[i].size = Vector2(14,3)
		keyled[i].color = Color(0.2, 0.2, 0.0, 1)
		keyled2[i].color = Color(0.2, 0.2, 0.0, 1)
		var j:int = i % 12
		if j == 1 or j == 4 or j == 6 or j == 9 or j == 11:
			# Black keys: position 50 pixels lower to overlap white keys
			keyboard[i].position = Vector2(pos_x-7,pos_y+0)
			# Set z_index higher than white keys so black keys handle clicks in overlap area
			keyboard[i].z_index = 1
			keyled[i].position = Vector2(0,-3)
			keyled2[i].position = Vector2(0,-3-3)
			keyon[i].size = Vector2(14,30)
			keyoff[i].size = Vector2(14,30)
			var shape:RectangleShape2D = RectangleShape2D.new()
			shape.size = Vector2(14,30)
			collision[i].set_shape(shape)
			keyon[i].color = Color(1.0, 0.0, 0.0, 1)
			keyoff[i].color = Color(0.0, 0.0, 0.0, 1)
			collision[i].position = Vector2(7,15)
		else:
			# White keys: keep original position, z_index lower than black keys
			keyboard[i].position = Vector2(pos_x,pos_y+0)
			keyboard[i].z_index = 0
			keyled[i].position = Vector2(0,60+1)
			keyled2[i].position = Vector2(0,60+1+3)
			keyon[i].size = Vector2(14,60)
			keyoff[i].size = Vector2(14,60)
			var shape:RectangleShape2D = RectangleShape2D.new()
			shape.size = Vector2(14,60)
			collision[i].set_shape(shape)
			keyon[i].color = Color(1.0, 0.0, 0.0, 1)
			keyoff[i].color = Color(1.0, 0.9, 0.9, 1)
			collision[i].position = Vector2(7,30)
			pos_x += 17
		keyon[i].visible = true
		keyoff[i].visible = true
		keyled[i].visible = true
		keyled2[i].visible = true
		keyon[i].set_mouse_filter(Control.MOUSE_FILTER_IGNORE)
		keyoff[i].set_mouse_filter(Control.MOUSE_FILTER_IGNORE)
		keyboard[i].set_script(load("res://KeyboadArea2D.gd"))
		add_child(keyboard[i])
		keyboard[i].owner = self
		get_node("KEY"+str(index)).add_child(keyon[i])
		keyon[i].owner = self
		get_node("KEY"+str(index)).add_child(keyoff[i])
		keyoff[i].owner = self
		get_node("KEY"+str(index)).add_child(keyled[i])
		keyled[i].owner = self
		get_node("KEY"+str(index)).add_child(keyled2[i])
		keyled2[i].owner = self
		get_node("KEY"+str(index)).add_child(collision[i])
		collision[i].owner = keyboard[i]
		index += 1

	# Add keyboard area for right-click detection (covers entire keyboard area including gaps, excluding LEDs)
	var keyboard_area:Area2D = Area2D.new()
	keyboard_area.name = "KeyboardArea"
	# Calculate keyboard area dimensions
	# Width: from first white key (pos_x=10) to last white key + key width
	var keyboard_width = (Globalv.num_keyboard_key - 1) * 17 + 14  # Last key doesn't add spacing
	# Height: from top of white keys to bottom of white keys (excluding LEDs)
	var keyboard_height = 60  # White key height only
	var keyboard_area_shape:CollisionShape2D = CollisionShape2D.new()
	var shape:RectangleShape2D = RectangleShape2D.new()
	shape.size = Vector2(keyboard_width, keyboard_height)
	keyboard_area_shape.set_shape(shape)
	keyboard_area_shape.position = Vector2(keyboard_width / 2, keyboard_height / 2)
	keyboard_area.position = Vector2(10, pos_y)  # Start from top of white keys
	keyboard_area.z_index = -1  # Behind keys so keys handle left clicks
	keyboard_area.set_script(load("res://KeyboardArea2D.gd"))
	add_child(keyboard_area)
	keyboard_area.owner = self
	keyboard_area.add_child(keyboard_area_shape)
	keyboard_area_shape.owner = keyboard_area


func keyboard_clear_if_requied()->void:
	if Globalv.is_keyboard_clear:
		Globalv.is_keyboard_clear = false
		var index:int = Globalv.most_left_key_num
		for i in range(Globalv.num_keyboard_key):
			var keyboard = get_node("KEY"+str(index))
			keyboard.get_node("keyoff").visible = true
			index += 1

		for i in range(256):
			var led = get_node("LED"+str(i)).get_node("Led") 
			led.color = Color(0.2, 0.2, 0, 1)

func _on_gd_synthesizer_note_changed(state, note)->void:
	keyboard_clear_if_requied()
		
	var inst = note["instrumentNum"]
	if not Globalv.is_percussion_prog_select:
		# ピアノロールの可視性を確認
		var piano_roll = get_node_or_null("PianoRoll")
		var is_piano_roll_visible = piano_roll != null and piano_roll.visible
		
		# パーカッションモード状態を取得
		var is_perc_mode = is_percussion_mode
		
		# ノートのチャンネルを確認
		var channel = note.get("channel", -1)
		var is_percussion_channel = (channel == 9 or channel == 25)
		
		# 使用するprogram値を決定
		var program_to_use: int = inst
		if is_percussion_channel:
			var gd_synthesizer = get_node_or_null("GDSynthesizer")
			if gd_synthesizer:
				var percussion_array = gd_synthesizer.percussion_params_array
				var key = note.get("key", -1)
				if percussion_array != null and percussion_array.size() > key and key >= 0:
					var percussion_item = percussion_array[key]
					if percussion_item != null and percussion_item.has("program"):
						program_to_use = percussion_item["program"]
		
		# 点灯中のprogramを追跡（チャンネル情報も保存）
		if state == "note_on":
			active_programs[program_to_use] = is_percussion_channel
		# note_offの時は、LEDがデフォルト色に戻った時に削除する
		# ここでは削除しない（複数のノートが同じprogramを使う可能性があるため）
		
		var color:Color = Color(0.2, 0.2, 0, 1)
		
		if (state == "note_on"):
			# 色の決定ロジック
			if not is_piano_roll_visible:
				# ピアノロールが非表示の場合：常に黄色
				color = Color(1.0, 1.0, 0.0, 1.0)
			else:
				# ピアノロールが表示されている場合
				if is_perc_mode:
					# パーカッションモード表示時
					if is_percussion_channel:
						# パーカッションノート → カラーテーブル
						var piano_roll_overlay = get_node_or_null("PianoRoll/TextureRect/PianoRollOverlayPercussion")
						if not piano_roll_overlay:
							piano_roll_overlay = get_node_or_null("PianoRoll/TextureRect/PianoRollOverlay")
						if piano_roll_overlay:
							color = piano_roll_overlay._get_program_color(program_to_use)
						else:
							color = Color(1.0, 1.0, 0.0, 1.0)  # フォールバック: 黄色
					else:
						# 非パーカッションノート → 黄色
						color = Color(1.0, 1.0, 0.0, 1.0)
				else:
					# 非パーカッションモード表示時
					if is_percussion_channel:
						# パーカッションノート → 黄色
						color = Color(1.0, 1.0, 0.0, 1.0)
					else:
						# 非パーカッションノート → カラーテーブル
						var piano_roll_overlay = get_node_or_null("PianoRoll/TextureRect/PianoRollOverlayNonPercussion")
						if not piano_roll_overlay:
							piano_roll_overlay = get_node_or_null("PianoRoll/TextureRect/PianoRollOverlay")
						if piano_roll_overlay:
							color = piano_roll_overlay._get_program_color(program_to_use)
						else:
							color = Color(1.0, 1.0, 0.0, 1.0)  # フォールバック: 黄色
		
		# LEDの色を更新（ピアノロールの可視性に関わらず更新）
		var led_index = program_to_use
		if led_index >= 0 and led_index < 256:
			var led_node = get_node_or_null("LED"+str(led_index))
			if led_node:
				led_node.get_node("Led").color = color
				# LEDがデフォルト色に戻った場合、active_programsから削除
				if color == Color(0.2, 0.2, 0, 1):
					if active_programs.has(program_to_use):
						active_programs.erase(program_to_use)

	if Globalv.is_percussion == 0:
		var key = note["key"]
		if 	Globalv.program == inst\
			and key >= Globalv.most_left_key_num and\
			key < Globalv.most_left_key_num+Globalv.num_keyboard_key+1:
			var keyboard = get_node("KEY"+str(key))
			if (state == "note_on"):
				keyboard.get_node("keyoff").visible = false
			else:
				keyboard.get_node("keyoff").visible = true
	elif note["channel"]==9 or note["channel"]==25:
		var key = note["key"]
		if 	key >= Globalv.most_left_key_num\
			and key < Globalv.most_left_key_num+Globalv.num_keyboard_key+1:
			var keyboard = get_node("KEY"+str(key))
			if (state == "note_on"):
				keyboard.get_node("keyoff").visible = false
			else:
				keyboard.get_node("keyoff").visible = true


func reset_percussion_related()->void:
	var index:int = Globalv.most_left_key_num
	for i in range(Globalv.num_keyboard_key):
		get_node("KEY"+str(index)).get_node("keyled").color \
			= Color(0.2, 0.2, 0.0, 1)
		get_node("KEY"+str(index)).get_node("keyled2").color \
			= Color(0.2, 0.2, 0.0, 1)
		index += 1
	index = Globalv.most_left_key_num
	for i in range(Globalv.num_keyboard_key):
		get_node("KEY"+str(index)).get_node("keyled").color \
			= Color(0.2, 0.2, 0.0, 1)
		get_node("KEY"+str(index)).get_node("keyled2").color \
			= Color(0.2, 0.2, 0.0, 1)
		index += 1
	for i in range(256):
		get_node("LED"+str(i)).get_node("Led").color \
			= Color(0.2, 0.2, 0.0, 1)
	Globalv.percussion_key = -1
	Globalv.percussion_prog = -1
	Globalv.percussion_note = -1
	Globalv.is_percussion_key_select = false
	Globalv.is_percussion_prog_select = false
	Globalv.is_percussion_note_select = false


func keyb_note_on(note)->void:
	if Globalv.is_percussion_key_select:
		Globalv.is_percussion_key_select = false
		Globalv.percussion_key = note
		$"GDSynthesizer".copy_percussion_params()
		var index:int = Globalv.most_left_key_num
		for i in range(Globalv.num_keyboard_key):
			get_node("KEY"+str(index)).get_node("keyled").color \
				= Color(0.2, 0.2, 0.0, 1)
			get_node("KEY"+str(index)).get_node("keyled2").color \
				= Color(0.2, 0.2, 0.0, 1)
			index += 1
		get_node("KEY"+str(Globalv.percussion_key)).get_node("keyled").color \
			= Color(0.0, 0.0, 1.0, 1)
		if Globalv.percussion_note != -1:
			get_node("KEY"+str(Globalv.percussion_note))\
				.get_node("keyled2").color = Color(0.0, 1.0, 0.0, 1)
		$"GDSynthesizer".program_change()
		draw_percussion_prog()

	if Globalv.is_percussion_note_select:
		Globalv.is_percussion_note_select = false
		Globalv.percussion_note = note
		$"GDSynthesizer".modify_percussion_params()
		var index:int = Globalv.most_left_key_num
		for i in range(Globalv.num_keyboard_key):
			get_node("KEY"+str(index)).get_node("keyled2").color \
				= Color(0.2, 0.2, 0.0, 1)
			index += 1
		if Globalv.percussion_note != -1:
			get_node("KEY"+str(Globalv.percussion_note))\
				.get_node("keyled2").color = Color(0.0, 1.0, 0.0, 1)
		$"GDSynthesizer".program_change()
		draw_percussion_prog()
			
	var channel:int = 25 # 9 and 25 are for percussions.
	if Globalv.is_percussion == 0:
		channel = 16
	var dic = {	"channel": channel, 
				"key": note,
				"velocity": 127, 
				"program": Globalv.program, 
				"tempo": Globalv.base_tempo}
	$"GDSynthesizer".set_note_on(dic)
	
func keyb_note_off(note)->void:
	var channel:int = 25 # 9 and 25 are for percussions.
	if Globalv.is_percussion == 0:
		channel = 16
	var dic = {	"channel": channel, 
				"key": note, 
				"velocity": 127, 
				"program": Globalv.program, 
				"tempo": Globalv.base_tempo}
	$"GDSynthesizer".set_note_off(dic)

func info_change()->void:
	redraw_signal_allocation_bar()
	var pkey = "--"
	if Globalv.percussion_key != -1:
		pkey = 	str(Globalv.percussion_key)\
				+Globalv.note_names[Globalv.percussion_key%12]\
				+str(int(float(Globalv.percussion_key)/12.0)-1)\
				+" "\
				+Globalv.percussions[Globalv.percussion_key]
	var pprog = "--"
	if Globalv.percussion_prog != -1:
		pprog = str(Globalv.percussion_prog+1)+" "\
		+Globalv.insturments[Globalv.percussion_prog]
	var pnote = "--"
	if Globalv.percussion_note != -1:
		pnote = str(Globalv.percussion_note)\
		+Globalv.note_names[Globalv.percussion_note%12]\
		+str(int(float(Globalv.percussion_note)/12.0)-1)
	$"LabelInfoProg".text = "PROG{ch}: {ins}\n".format({
			"ch":Globalv.program+1,
			"ins":Globalv.insturments[Globalv.program]})
	$"LabelInfoPercussion".text = \
			  "KEY: {pkey}\n".format({"pkey":pkey})\
			+ "PROG: {pprog}\n".format({"pprog":pprog})\
			+ "NOTE: {pnote}".format({"pnote":pnote})
	$"LabelInfoSMF".text = "SMF: {path}".format(
		{"path":Globalv.smf_filename})


func _input(event:InputEvent)->void:
	if event is InputEventKey:
		if event.pressed:
			if event.keycode == KEY_SHIFT and is_sift == false:
				is_sift = true	
			if event.keycode == KEY_Z and keyb[0] == false:
				keyb[0] = true	
				keyb_note_on(60)
			if event.keycode == KEY_S and keyb[1] == false:
				keyb[1] = true	
				keyb_note_on(61)
			if event.keycode == KEY_X and keyb[2] == false:
				keyb[2] = true	
				keyb_note_on(62)
			if event.keycode == KEY_D and keyb[3] == false:
				keyb[3] = true	
				keyb_note_on(63)
			if event.keycode == KEY_C and keyb[4] == false:
				keyb[4] = true	
				keyb_note_on(64)
			if event.keycode == KEY_V and keyb[5] == false:
				keyb[5] = true	
				keyb_note_on(65)
			if event.keycode == KEY_G and keyb[6] == false:
				keyb[6] = true	
				keyb_note_on(66)
			if event.keycode == KEY_B and keyb[7] == false:
				keyb[7] = true	
				keyb_note_on(67)
			if event.keycode == KEY_H and keyb[8] == false:
				keyb[8] = true	
				keyb_note_on(68)
			if event.keycode == KEY_N and keyb[9] == false:
				keyb[9] = true	
				keyb_note_on(69)
			if event.keycode == KEY_J and keyb[10] == false:
				keyb[10] = true	
				keyb_note_on(70)
			if event.keycode == KEY_M and keyb[11] == false:
				keyb[11] = true	
				keyb_note_on(71)
			if event.keycode == KEY_COMMA  and keyb[11] == false:
				keyb[11] = true
				keyb_note_on(72)
		else:
			if event.keycode == KEY_Z and keyb[0] == true:
				keyb[0] = false
				keyb_note_off(60)
			if event.keycode == KEY_S and keyb[1] == true:
				keyb[1] = false
				keyb_note_off(61)
			if event.keycode == KEY_X and keyb[2] == true:
				keyb[2] = false
				keyb_note_off(62)
			if event.keycode == KEY_D and keyb[3] == true:
				keyb[3] = false
				keyb_note_off(63)
			if event.keycode == KEY_C and keyb[4] == true:
				keyb[4] = false
				keyb_note_off(64)
			if event.keycode == KEY_V and keyb[5] == true:
				keyb[5] = false
				keyb_note_off(65)
			if event.keycode == KEY_G and keyb[6] == true:
				keyb[6] = false
				keyb_note_off(66)
			if event.keycode == KEY_B and keyb[7] == true:
				keyb[7] = false
				keyb_note_off(67)
			if event.keycode == KEY_H and keyb[8] == true:
				keyb[8] = false
				keyb_note_off(68)
			if event.keycode == KEY_N and keyb[9] == true:
				keyb[9] = false
				keyb_note_off(69)
			if event.keycode == KEY_J and keyb[10] == true:
				keyb[10] = false
				keyb_note_off(70)
			if event.keycode == KEY_M and keyb[11] == true:
				keyb[11] = false
				keyb_note_off(71)
			if event.keycode == KEY_COMMA  and keyb[11] == true:
				keyb[11] = false
				keyb_note_off(72)


func _on_gd_synthesizer_level_info(level)->void:
	var max_level:int = mini(int(level["max_level"]),1500)
	$"Bar_brawn".visible = true
	$"Bar_brawn".size = Vector2(int((float(max_level)/1500.0)*bar_size_x),5)

	var frame_level:int = mini(int(level["frame_level"]),1500)
	$"Bar_red".visible = true
	$"Bar_red".size = Vector2(int((float(frame_level)/1500.0)*bar_size_x),5)

	frame_level = mini(int(level["frame_level"]),1000)
	$"Bar_green".visible = true
	$"Bar_green".size = Vector2(int((float(frame_level)/1500.0)*bar_size_x),5)


func _on_button_key_select_pressed()->void:
	if    Globalv.is_percussion_key_select == false\
	  and Globalv.is_percussion_prog_select == false\
	  and Globalv.is_percussion_note_select == false:
		Globalv.is_percussion_key_select = true
		var index:int = Globalv.most_left_key_num
		for i in range(Globalv.num_keyboard_key):
			get_node("KEY"+str(index)).get_node("keyled").color \
				= Color(0.0, 0.0, 1.0, 1)
			index += 1
	else:
		Globalv.is_percussion_key_select = false
		var index:int = Globalv.most_left_key_num
		for i in range(Globalv.num_keyboard_key):
			get_node("KEY"+str(index))\
				.get_node("keyled").color = Color(0.2, 0.2, 0.0, 1)
			get_node("KEY"+str(index))\
				.get_node("keyled2").color = Color(0.2, 0.2, 0.0, 1)
			index += 1
		if Globalv.percussion_key != -1:
			get_node("KEY"+str(Globalv.percussion_key))\
				.get_node("keyled").color = Color(0.0, 0.0, 1.0, 1)
		if Globalv.percussion_note != -1:
			get_node("KEY"+str(Globalv.percussion_note))\
				.get_node("keyled2").color = Color(0.0, 1.0, 0.0, 1)


func piano_roll_background_preparation()->void:
	# Create background ColorRect to hide objects behind piano roll
	piano_roll_background = ColorRect.new()
	piano_roll_background.name = "PianoRollBackground"
	# Use background color matching the default viewport background (#4C4C4C)
	# #4C4C4C = RGB(76, 76, 76) = Color(76.0/255.0, 76.0/255.0, 76.0/255.0, 1.0)
	piano_roll_background.color = Color(76.0/255.0, 76.0/255.0, 76.0/255.0, 1.0)  # #4C4C4C
	piano_roll_background.z_index = 999  # Behind piano roll (1000) but above other elements
	piano_roll_background.visible = false  # Initially hidden
	add_child(piano_roll_background)
	piano_roll_background.owner = self

func toggle_piano_roll()->void:
	var piano_roll = get_node_or_null("PianoRoll")
	if piano_roll:
		piano_roll.visible = not piano_roll.visible
		# Also toggle background visibility
		if piano_roll_background:
			piano_roll_background.visible = piano_roll.visible
			# Update size and position when toggling (in case piano roll size changed)
			if piano_roll.visible:
				var piano_roll_texture_rect = get_node_or_null("PianoRoll/TextureRect")
				if piano_roll_texture_rect:
					var viewport = get_viewport()
					var screen_width = viewport.get_visible_rect().size.x
					var piano_roll_height = piano_roll_texture_rect.size.y
					var piano_roll_y = piano_roll.position.y
					# Cover from screen left edge to screen right edge
					var background_width = screen_width
					var background_x = 0  # Start from screen left edge
					
					piano_roll_background.size = Vector2(background_width, piano_roll_height)
					piano_roll_background.position = Vector2(background_x, piano_roll_y)
				
				# ピアノロールが表示されたときに、Aボタンの状態を更新
				var piano_roll_overlay_non_perc = get_node_or_null("PianoRoll/TextureRect/PianoRollOverlayNonPercussion")
				if piano_roll_overlay_non_perc:
					var show_all = piano_roll_overlay_non_perc.get_show_all_programs()
					update_allprograms_button_state(show_all)
				else:
					# フォールバック: 既存のPianoRollOverlay（互換性のため）
					var piano_roll_overlay = get_node_or_null("PianoRoll/TextureRect/PianoRollOverlay")
					if piano_roll_overlay and piano_roll_overlay.has_method("get_show_all_programs"):
						var show_all = piano_roll_overlay.get_show_all_programs()
						update_allprograms_button_state(show_all)
		
		# ピアノロールの表示/非表示を切り替えた時、点灯中のPROGRAM LEDの色を更新
		update_active_program_leds_color()

func update_piano_roll_background_size()->void:
	if piano_roll_background and piano_roll_background.visible:
		var piano_roll = get_node_or_null("PianoRoll")
		var piano_roll_texture_rect = get_node_or_null("PianoRoll/TextureRect")
		if piano_roll and piano_roll_texture_rect:
			var viewport = get_viewport()
			var screen_width = viewport.get_visible_rect().size.x
			var piano_roll_height = piano_roll_texture_rect.size.y
			var piano_roll_y = piano_roll.position.y
			# Cover from screen left edge to screen right edge
			var background_width = screen_width
			var background_x = 0  # Start from screen left edge
			
			piano_roll_background.size = Vector2(background_width, piano_roll_height)
			piano_roll_background.position = Vector2(background_x, piano_roll_y)

func _on_button_pianoroll_pressed()->void:
	toggle_piano_roll()

func _on_button_percussion_mode_pressed()->void:
	var button = get_node_or_null("ControlPercussion/ButtonPercussionMode")
	if button:
		# Globalv.is_percussionを切り替え（0 <-> 1）
		if Globalv.is_percussion == 0:
			Globalv.is_percussion = 1
		else:
			Globalv.is_percussion = 0
		
		# キーボードをクリア
		Globalv.is_keyboard_clear = true
		
		# ボタンのスタイルを更新
		update_percussion_mode_button_style()

func update_percussion_mode_button_style()->void:
	# Globalv.is_percussionの値に応じてボタンのスタイルを更新
	var button = get_node_or_null("ControlPercussion/ButtonPercussionMode")
	if not button:
		return
	
	if Globalv.is_percussion != 0:
		# パーカッション用：黄色地に黒文字
		var style_box = StyleBoxFlat.new()
		style_box.bg_color = Color(1.0, 1.0, 0.0, 1.0)  # 黄色
		button.add_theme_stylebox_override("normal", style_box)
		button.add_theme_stylebox_override("hover", style_box)
		button.add_theme_stylebox_override("pressed", style_box)
		# 文字色を黒に変更（すべての状態で）
		button.add_theme_color_override("font_color", Color(0.0, 0.0, 0.0, 1.0))
		button.add_theme_color_override("font_hover_color", Color(0.0, 0.0, 0.0, 1.0))
		button.add_theme_color_override("font_pressed_color", Color(0.0, 0.0, 0.0, 1.0))
		button.add_theme_color_override("font_focus_color", Color(0.0, 0.0, 0.0, 1.0))
		button.add_theme_color_override("font_disabled_color", Color(0.0, 0.0, 0.0, 1.0))
	else:
		# 非パーカッション用：黒地に白文字
		var style_box = StyleBoxFlat.new()
		style_box.bg_color = Color(0.0, 0.0, 0.0, 1.0)  # 黒
		button.add_theme_stylebox_override("normal", style_box)
		button.add_theme_stylebox_override("hover", style_box)
		button.add_theme_stylebox_override("pressed", style_box)
		# 文字色を白に変更（すべての状態で）
		button.add_theme_color_override("font_color", Color(1.0, 1.0, 1.0, 1.0))
		button.add_theme_color_override("font_hover_color", Color(1.0, 1.0, 1.0, 1.0))
		button.add_theme_color_override("font_pressed_color", Color(1.0, 1.0, 1.0, 1.0))
		button.add_theme_color_override("font_focus_color", Color(1.0, 1.0, 1.0, 1.0))
		button.add_theme_color_override("font_disabled_color", Color(1.0, 1.0, 1.0, 1.0))

func _process(_delta):
	# Globalv.is_percussionの変化を監視してボタンの状態を更新
	if last_is_percussion != Globalv.is_percussion:
		last_is_percussion = Globalv.is_percussion
		update_percussion_mode_button_style()

var is_percussion_mode: bool = false  # パーカッションモードフラグ
var active_programs: Dictionary = {}  # 現在点灯中のprogram値とチャンネル情報のマッピング {program: is_percussion_channel}
var last_is_percussion: int = -1  # Globalv.is_percussionの前回値を監視

func get_is_percussion_mode() -> bool:
	return is_percussion_mode

func _on_button_percussionroll_pressed()->void:
	var button = get_node_or_null("ControlPercussion/ButtonPercussionroll")
	if button:
		is_percussion_mode = not is_percussion_mode
		print("ButtonPercussionroll pressed, is_percussion_mode: ", is_percussion_mode)
		if is_percussion_mode:
			# 黄色に変更（StyleBoxFlatを使用）
			var style_box = StyleBoxFlat.new()
			style_box.bg_color = Color(1.0, 1.0, 0, 1)
			button.add_theme_stylebox_override("normal", style_box)
			button.add_theme_stylebox_override("hover", style_box)
			button.add_theme_stylebox_override("pressed", style_box)
			# 文字色を黒に変更（すべての状態で）
			button.add_theme_color_override("font_color", Color(0.0, 0.0, 0.0, 1.0))
			button.add_theme_color_override("font_hover_color", Color(0.0, 0.0, 0.0, 1.0))
			button.add_theme_color_override("font_pressed_color", Color(0.0, 0.0, 0.0, 1.0))
			button.add_theme_color_override("font_focus_color", Color(0.0, 0.0, 0.0, 1.0))
			button.add_theme_color_override("font_disabled_color", Color(0.0, 0.0, 0.0, 1.0))
		else:
			# 通常色に戻す
			button.remove_theme_stylebox_override("normal")
			button.remove_theme_stylebox_override("hover")
			button.remove_theme_stylebox_override("pressed")
			# 文字色を明示的に白に設定（removeでは戻らないため）
			button.add_theme_color_override("font_color", Color(1.0, 1.0, 1.0, 1.0))
			button.add_theme_color_override("font_hover_color", Color(1.0, 1.0, 1.0, 1.0))
			button.add_theme_color_override("font_pressed_color", Color(1.0, 1.0, 1.0, 1.0))
			button.add_theme_color_override("font_focus_color", Color(1.0, 1.0, 1.0, 1.0))
			button.add_theme_color_override("font_disabled_color", Color(1.0, 1.0, 1.0, 1.0))
		
		# 対応するPianoRollOverlayを可視化/不可視化
		var piano_roll_overlay_non_perc = get_node_or_null("PianoRoll/TextureRect/PianoRollOverlayNonPercussion")
		var piano_roll_overlay_perc = get_node_or_null("PianoRoll/TextureRect/PianoRollOverlayPercussion")
		
		if is_percussion_mode:
			# パーカッションモード: パーカッション用を可視化、非パーカッション用を不可視化
			if piano_roll_overlay_perc:
				piano_roll_overlay_perc.visible = true
			if piano_roll_overlay_non_perc:
				piano_roll_overlay_non_perc.visible = false
		else:
			# 非パーカッションモード: 非パーカッション用を可視化、パーカッション用を不可視化
			if piano_roll_overlay_non_perc:
				piano_roll_overlay_non_perc.visible = true
			if piano_roll_overlay_perc:
				piano_roll_overlay_perc.visible = false
		
		# ピアノロールが表示されている場合、点灯中のPROGRAM LEDの色を更新
		var piano_roll = get_node_or_null("PianoRoll")
		if piano_roll and piano_roll.visible:
			update_active_program_leds_color()
	else:
		print("ButtonPercussionroll not found!")

func update_active_program_leds_color()->void:
	# 点灯中の全てのprogram値に対して、新しい可視性状態に応じた色を計算して設定
	var piano_roll = get_node_or_null("PianoRoll")
	var is_piano_roll_visible = piano_roll != null and piano_roll.visible
	
	var is_perc_mode = is_percussion_mode
	
	# active_programsに登録されている全てのprogram値に対して色を更新
	for program_value in active_programs.keys():
		var is_percussion_channel = active_programs[program_value]
		var color: Color = Color(0.2, 0.2, 0, 1)
		
		# 色の決定ロジック
		if not is_piano_roll_visible:
			# ピアノロールが非表示の場合：常に黄色
			color = Color(1.0, 1.0, 0.0, 1.0)
		else:
			# ピアノロールが表示されている場合
			if is_perc_mode:
				# パーカッションモード表示時
				if is_percussion_channel:
					# パーカッションノート → カラーテーブル
					var piano_roll_overlay = get_node_or_null("PianoRoll/TextureRect/PianoRollOverlayPercussion")
					if not piano_roll_overlay:
						piano_roll_overlay = get_node_or_null("PianoRoll/TextureRect/PianoRollOverlay")
					if piano_roll_overlay:
						color = piano_roll_overlay._get_program_color(program_value)
					else:
						color = Color(1.0, 1.0, 0.0, 1.0)  # フォールバック: 黄色
				else:
					# 非パーカッションノート → 黄色
					color = Color(1.0, 1.0, 0.0, 1.0)
			else:
				# 非パーカッションモード表示時
				if is_percussion_channel:
					# パーカッションノート → 黄色
					color = Color(1.0, 1.0, 0.0, 1.0)
				else:
					# 非パーカッションノート → カラーテーブル
					var piano_roll_overlay = get_node_or_null("PianoRoll/TextureRect/PianoRollOverlayNonPercussion")
					if not piano_roll_overlay:
						piano_roll_overlay = get_node_or_null("PianoRoll/TextureRect/PianoRollOverlay")
					if piano_roll_overlay:
						color = piano_roll_overlay._get_program_color(program_value)
					else:
						color = Color(1.0, 1.0, 0.0, 1.0)  # フォールバック: 黄色
		
		# LEDの色を更新
		if program_value >= 0 and program_value < 256:
			var led_node = get_node_or_null("LED"+str(program_value))
			if led_node:
				var led = led_node.get_node_or_null("Led")
				if led:
					led.color = color

func _on_button_allprograms_pressed()->void:
	var button = get_node_or_null("ControlPercussion/ButtonAllPrograms")
	if button:
		# 非パーカッション用Overlayの_toggle_display_mode()を呼び出す
		var piano_roll_overlay_non_perc = get_node_or_null("PianoRoll/TextureRect/PianoRollOverlayNonPercussion")
		if piano_roll_overlay_non_perc:
			piano_roll_overlay_non_perc._toggle_display_mode()
		else:
			# フォールバック: 既存のPianoRollOverlay（互換性のため）
			var piano_roll_overlay = get_node_or_null("PianoRoll/TextureRect/PianoRollOverlay")
			if piano_roll_overlay and not piano_roll_overlay.is_percussion_overlay:
				piano_roll_overlay._toggle_display_mode()
	else:
		print("ButtonAllPrograms not found!")

func update_allprograms_button_state(show_all: bool)->void:
	# ボタンの視覚的状態を更新
	var button = get_node_or_null("ControlPercussion/ButtonAllPrograms")
	if button:
		if show_all:
			# 黄色に変更（StyleBoxFlatを使用）
			var style_box = StyleBoxFlat.new()
			style_box.bg_color = Color(1.0, 1.0, 0, 1)
			button.add_theme_stylebox_override("normal", style_box)
			button.add_theme_stylebox_override("hover", style_box)
			button.add_theme_stylebox_override("pressed", style_box)
			# 文字色を黒に変更（すべての状態で）
			button.add_theme_color_override("font_color", Color(0.0, 0.0, 0.0, 1.0))
			button.add_theme_color_override("font_hover_color", Color(0.0, 0.0, 0.0, 1.0))
			button.add_theme_color_override("font_pressed_color", Color(0.0, 0.0, 0.0, 1.0))
			button.add_theme_color_override("font_focus_color", Color(0.0, 0.0, 0.0, 1.0))
			button.add_theme_color_override("font_disabled_color", Color(0.0, 0.0, 0.0, 1.0))
		else:
			# 通常色に戻す
			button.remove_theme_stylebox_override("normal")
			button.remove_theme_stylebox_override("hover")
			button.remove_theme_stylebox_override("pressed")
			# 文字色を明示的に白に設定（removeでは戻らないため）
			button.add_theme_color_override("font_color", Color(1.0, 1.0, 1.0, 1.0))
			button.add_theme_color_override("font_hover_color", Color(1.0, 1.0, 1.0, 1.0))
			button.add_theme_color_override("font_pressed_color", Color(1.0, 1.0, 1.0, 1.0))
			button.add_theme_color_override("font_focus_color", Color(1.0, 1.0, 1.0, 1.0))
			button.add_theme_color_override("font_disabled_color", Color(1.0, 1.0, 1.0, 1.0))

func _on_button_regist_program_pressed()->void:
	if    Globalv.is_percussion_key_select == false\
	  and Globalv.is_percussion_prog_select == false\
	  and Globalv.is_percussion_note_select == false\
	  and Globalv.percussion_key != -1:
		Globalv.is_percussion_prog_select = true
		for i in range(256):
			get_node("LED"+str(i)).get_node("Led").color \
				= Color(0.0, 1.0, 0.0, 1)
	else:
		Globalv.is_percussion_prog_select = false
		for i in range(256):
			get_node("LED"+str(i)).get_node("Led").color \
				= Color(0.2, 0.2, 0.0, 1)


func _on_button_regist_note_pressed()->void:
	if    Globalv.is_percussion_key_select == false\
	  and Globalv.is_percussion_prog_select == false\
	  and Globalv.is_percussion_note_select == false\
	  and Globalv.percussion_key != -1:
		Globalv.is_percussion_note_select = true
		var index:int = Globalv.most_left_key_num
		for i in range(Globalv.num_keyboard_key):
			get_node("KEY"+str(index)).get_node("keyled2").color \
				= Color(0.0, 1.0, 0.0, 1)
			index += 1
	else:
		Globalv.is_percussion_note_select = false
		var index:int = Globalv.most_left_key_num
		for i in range(Globalv.num_keyboard_key):
			get_node("KEY"+str(index)).get_node("keyled2").color \
				= Color(0.2, 0.2, 0.0, 1)
			index += 1
		if Globalv.percussion_note != -1:
			get_node("KEY"+str(Globalv.percussion_note)).\
				get_node("keyled2").color = Color(0.0, 1.0, 0.0, 1)

func draw_percussion_prog()->void:
	if Globalv.percussion_prog != -1:
		var k:int = int(float(Globalv.percussion_prog)/128.0)
		var j:int = int(float(Globalv.percussion_prog%128)/8.0)
		var i:int = int(Globalv.percussion_prog%8)
		var pled_position:Vector2 = Vector2(
			(5+2)*(j*(8+1)+i)+12+1,
			(5+12)*k+Globalv.pled_pos_y+10+7)
		$"PLed2".set("position", pled_position)
		$"PLed2".color = Color(0.0, 1.0, 0.0, 1)

func all_led_off()->void:
	for i in range(256):
		get_node("LED"+str(i)).get_node("Led").color = Color(0.2, 0.2, 0.0, 1)
	draw_percussion_prog()
	



func _on_button_licens_pressed():
	$"ControlLicens".visible=true
	var path="res://license.txt"
	var f:FileAccess = FileAccess.open(path, FileAccess.READ)
	var content = f.get_as_text()
	f.close()
	$"ControlLicens/TextEdit".text=content


func _on_button_pressed():
	$"ControlLicens".visible=false
