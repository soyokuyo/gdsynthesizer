extends Area2D

func changeKnob(vfloor:float, ceiling:float, head:float, 
		tail:float, value:Variant)->void:
	var angle:float = float(value)*(tail-head)/(ceiling-vfloor)+head
	angle = angle / 180.0 * PI
	if typeof(value) == TYPE_INT: $Label.text = "%5d" % value
	elif typeof(value) == TYPE_FLOAT: $Label.text = "%4.2f" % value
	$Sprite2D.rotation = angle

func knob_draw()->void:
	var instance_name:StringName = $"..".get_name()
	match instance_name:
		"KnobNumPoly":
			changeKnob(0.0, 50.0, -160.0, 160.0, Globalv.num_division) 
		"KnobBaseTempo":
			changeKnob(0.0, 300.0, -156.0, 156.0, Globalv.base_tempo) 
		"KnobProgramSelect":
			changeKnob(0.0, 256.0, -156.0, 156.0, Globalv.program+1) 
			$"../..".info_change()
			var k:int = int(float(Globalv.program)/128.0)
			var j:int = int(float(Globalv.program%128)/8.0)
			var i:int = int(Globalv.program%8)
			var pled_position:Vector2 = Vector2(
				(5+2)*(j*(8+1)+i)+12,
				(5+12)*k+Globalv.pled_pos_y+10)
			$"../../PLed".set("position", pled_position)
		"KnobpreOnTime":
			changeKnob(0.0, 5000.0, -156.0, 156.0, int(Globalv.pre_on_time)) 
		"KnobAtackSlopeTime":
			changeKnob(0.0, 5000.0, 0.0, 720.0, int(Globalv.atack_slope_time)) 
		"KnobDecayHalfLifeTime":
			changeKnob(0.0, 5000.0, 0.0, 720.0, int(Globalv.decay_half_life_time)) 
		"KnobSustainRate":
			changeKnob(0.0, 1.0, -156.0, 156.0, Globalv.sustain_rate) 
		"KnobReleaseSlopeTime":
			changeKnob(0.0, 5000.0, 0.0, 720.0, int(Globalv.release_slope_time)) 
		"KnobTotalSignalLevel":
			changeKnob(0.0, 1.0, -156.0, 156.0, Globalv.total_gain) 
		"KnobW1vsOthersRatio":
			changeKnob(0.0, 1.0, -156.0, 156.0, Globalv.base_vs_others_ratio) 
		"KnobW2vsW3Ratio":
			changeKnob(0.0, 1.0, -156.0, 156.0, Globalv.side1_vs_side2_ratio) 
		"KnobNoiseVsWaveRatio":
			changeKnob(0.0, 1.0, -156.0, 156.0, Globalv.noise_ratio) 
		"KnobW1Waveform":
			changeKnob(0.0, 5.0, 15.0, 140.0, Globalv.base_wave1+1) 
			$"../../GDSynthesizer".get_mini_wave_image(
				Globalv.base_wave1, 
				0,
				$"../../TextureRectW1")
		"KnobW1OffsetCent":
			changeKnob(-6000.0, 8400.0, 360.0, 1260.0, int(Globalv.base_offset_cent1)) 
		"KnobW2Waveform":
			changeKnob(0.0, 5.0, 15.0, 140.0, Globalv.base_wave2+1) 
			$"../../GDSynthesizer".get_mini_wave_image(
				Globalv.base_wave2, 
				0,
				$"../../TextureRectW2")
		"KnobW2OffsetCent":
			changeKnob(-6000.0, 8400.0, 360.0, 1260.0, int(Globalv.base_offset_cent2)) 
		"KnobW3Waveform":
			changeKnob(0.0, 5.0, 15.0, 140.0, Globalv.base_wave3+1) 
			$"../../GDSynthesizer".get_mini_wave_image(
				Globalv.base_wave3, 
				0,
				$"../../TextureRectW3")
		"KnobW3OffsetCent":
			changeKnob(-6000.0, 8400.0, 360.0, 1260.0, int(Globalv.base_offset_cent3)) 
		"KnobAddedNoiseColor":
			changeKnob(0.0, 2.0, 12.0, 115.0, Globalv.noise_color_type+1) 
		"KnobD1DelayTime":
			changeKnob(0.0, 500.0, 0.0, 360.0, int(Globalv.delay0_time)) 
		"KnobD1Level":
			changeKnob(0.0, 0.2, -156.0, 156.0, Globalv.delay0_ratio) 
		"KnobD2DelayTime":
			changeKnob(0.0, 500.0, 0.0, 360.0, int(Globalv.delay1_time)) 
		"KnobD2Level":
			changeKnob(0.0, 0.2, -156.0, 156.0, Globalv.delay1_ratio) 
		"KnobD3DelayTime":
			changeKnob(0.0, 500.0, 0.0, 360.0, int(Globalv.delay2_time)) 
		"KnobD3Level":
			changeKnob(0.0, 0.2, -156.0, 156.0, Globalv.delay2_ratio) 
		"KnobFDNDistributionType":
			changeKnob(0.0, 3.0, 8.0, 128.0, Globalv.freq_noise_type+1) 
		"KnobFDNCentRange":
			changeKnob(0.0, 8400.0, 0.0, 1260.0, int(Globalv.freq_noise_cent_range)) 
		"KnobFMCarrierFreq":
			changeKnob(0.0, 880.0, 0.0, 720.0, int(Globalv.fm_freq)) 
		"KnobFMCentRange":
			changeKnob(-2400.0, 8400.0, 360.0, 1260.0, int(Globalv.fm_cent_range))
		"KnobFMPhaseOffset":
			changeKnob(0.0, 360.0, -156.0, 156.0, int(Globalv.fm_phase_offset*180.0)) 
			$"../../GDSynthesizer".get_mini_wave_image(
				Globalv.fm_wave if Globalv.fm_wave != 4 else 5, 
				int(Globalv.fm_phase_offset*180.0),
				$"../../TextureRectLFOFM")
		"KnobFMCarrierWaveform":
			changeKnob(0.0, 5.0, 15.0, 140.0, Globalv.fm_wave+1) 
			$"../../GDSynthesizer".get_mini_wave_image(
				Globalv.fm_wave if Globalv.fm_wave != 4 else 5, 
				int(Globalv.fm_phase_offset*180.0),
				$"../../TextureRectLFOFM")
		"KnobFMSyncWithTempo":
			changeKnob(0.0, 2.0, -15.0, 125.0, -Globalv.fm_sync+2) 
		"KnobAMCarrierFreq":
			changeKnob(0.0, 880.0, 0.0, 720.0, int(Globalv.am_freq)) 
		"KnobAMAmplitudeLevel":
			changeKnob(0.0, 1.0, -156.0, 156.0, Globalv.am_level) 
		"KnobAMPhaseOffset":
			changeKnob(0.0, 360.0, -156.0, 156.0, int(Globalv.am_phase_offset*180.0)) 
			$"../../GDSynthesizer".get_mini_wave_image(
				Globalv.am_wave if Globalv.am_wave != 4 else 5, 
				int(Globalv.am_phase_offset*180.0),
				$"../../TextureRectLFOAM")
		"KnobAMCarrierWaveform":
			changeKnob(0.0, 5.0, 15.0, 140.0, Globalv.am_wave+1) 
			$"../../GDSynthesizer".get_mini_wave_image(
				Globalv.am_wave if Globalv.am_wave != 4 else 5, 
				int(Globalv.am_phase_offset*180.0),
				$"../../TextureRectLFOAM")
		"KnobAMSyncWithTempo":
			changeKnob(0.0, 2.0, -15.0, 125.0, -Globalv.am_sync+2) 
		var n : print(n, " is not used")


func _ready()->void:
	var instance_name:StringName = $"..".get_name()
	if instance_name != "Knob": Globalv.num_elements += 1
	knob_draw()


@warning_ignore("unused_parameter")
func _process(delta:float)->void:
	var instance_name:StringName = $"..".get_name()
	if instance_name != "Knob":
		if Globalv.is_loaded == true:
			Globalv.num_execution += 1
			knob_draw()
			if Globalv.num_elements == Globalv.num_execution:
				Globalv.is_loaded = false
	else: knob_draw()


func getAmount(fine:Variant, coarse:Variant):
	if typeof(fine) == TYPE_INT:
		var amount:int = fine
		if Input.is_action_pressed("key_shift"): amount = int(coarse)
		return amount
	elif typeof(fine) == TYPE_FLOAT:
		var amount:float = fine
		if Input.is_action_pressed("key_shift"): amount = float(coarse)
		return amount
	else: print("getAmount: invalid input")


@warning_ignore("unused_parameter")
func _on_input_event(viewport:Node, event:InputEvent , shape_idx:int)->void:
	var direction:int = 0
	if event is InputEventMouseButton and event.pressed:
		var instance_name = $"..".get_name()
		if event.button_index == MOUSE_BUTTON_WHEEL_UP: direction = 1
		elif event.button_index == MOUSE_BUTTON_WHEEL_DOWN: direction = -1
		if direction != 0:
			match instance_name:
				"KnobNumPoly":
					var value:float = Globalv.num_division
					value += float(direction) * getAmount(0.1, 1.0)
					value  = clampf(value, 1.0, 50.0)
					Globalv.num_division = value
				"KnobBaseTempo":
					var value:int = Globalv.base_tempo
					value += direction * getAmount(1, 10)
					value  = clampi(value, 1, 300)
					Globalv.base_tempo = value
				"KnobProgramSelect":
					var value:int = Globalv.program+1
					value += direction * getAmount(1, 10)
					value  = clampi(value, 1, 256)
					Globalv.program = value-1
					$"../../GDSynthesizer".program_change()
					Globalv.is_keyboard_clear = true
				"KnobpreOnTime":
					# Only allow changes when SMF file is not loaded
					if Globalv.smf_filename == "":
						var value: float = Globalv.pre_on_time
						value += float(direction) * 100.0  # Change by 100 units
						value = clampf(value, 0.0, 5000.0)
						Globalv.pre_on_time = value
						
						# Reflect changes to GDSynthesizer
						var gd_synthesizer = $"../../GDSynthesizer"
						if gd_synthesizer:
							var ctr_params = gd_synthesizer.get_control_params()
							ctr_params["preOnTime"] = Globalv.pre_on_time
							gd_synthesizer.set_control_params(ctr_params)
						
						# Notify FrontCase
						var front_case = $"../.."
						if front_case and front_case.has_method("update_piano_roll_pre_on_time"):
							front_case.update_piano_roll_pre_on_time()
					else:
						return  # Silently ignore when SMF file is loaded
				"KnobAtackSlopeTime":
					var value:int = int(Globalv.atack_slope_time)
					value += direction * getAmount(1, 100)
					value  = clampi(value, 1, 5000)
					Globalv.atack_slope_time = float(value)
				"KnobDecayHalfLifeTime":
					var value:int = int(Globalv.decay_half_life_time)
					value += direction * getAmount(1, 100)
					value  = clampi(value, 1, 5000)
					Globalv.decay_half_life_time = float(value)
				"KnobSustainRate":
					var value:float = Globalv.sustain_rate
					value += float(direction) * getAmount(0.01, 0.1)
					value  = clampf(value, 0.0, 1.0)
					Globalv.sustain_rate = value
				"KnobReleaseSlopeTime":
					var value:int = int(Globalv.release_slope_time)
					value += direction * getAmount(1, 100)
					value  = clampi(value, 1, 5000)
					Globalv.release_slope_time = float(value)
				"KnobTotalSignalLevel":
					var value:float = Globalv.total_gain
					value += float(direction) * getAmount(0.01, 0.1)
					value  = clampf(value, 0.0, 1.0)
					Globalv.total_gain = value
				"KnobW1vsOthersRatio":
					var value:float = Globalv.base_vs_others_ratio
					value += float(direction) * getAmount(0.01, 0.1)
					value  = clampf(value, 0.0, 1.0)
					Globalv.base_vs_others_ratio = value
				"KnobW2vsW3Ratio":
					var value:float = Globalv.side1_vs_side2_ratio
					value += float(direction) * getAmount(0.01, 0.1)
					value  = clampf(value, 0.0, 1.0)
					Globalv.side1_vs_side2_ratio = value
				"KnobNoiseVsWaveRatio":
					var value:float = Globalv.noise_ratio
					value += float(direction) * getAmount(0.01, 0.1)
					value  = clampf(value, 0.0, 1.0)
					Globalv.noise_ratio = value
				"KnobW1Waveform":
					var value:int = Globalv.base_wave1+1
					value += direction * getAmount(1, 1)
					value  = clampi(value, 1, 5)
					Globalv.base_wave1 = value-1
				"KnobW1OffsetCent":
					var value:int = int(Globalv.base_offset_cent1)
					value += direction * getAmount(1, 100)
					value  = clampi(value, -6000, 8400)
					Globalv.base_offset_cent1 = float(value)
				"KnobW2Waveform":
					var value:int = Globalv.base_wave2+1
					value += direction * getAmount(1, 1)
					value  = clampi(value, 1, 5)
					Globalv.base_wave2 = value-1
				"KnobW2OffsetCent":
					var value:int = int(Globalv.base_offset_cent2)
					value += direction * getAmount(1, 100)
					value  = clampi(value, -6000, 8400)
					Globalv.base_offset_cent2 = float(value)
				"KnobW3Waveform":
					var value:int = Globalv.base_wave3+1
					value += direction * getAmount(1, 1)
					value  = clampi(value, 1, 5)
					Globalv.base_wave3 = value-1
				"KnobW3OffsetCent":
					var value:int = int(Globalv.base_offset_cent3)
					value += direction * getAmount(1, 100)
					value  = clampi(value, -6000, 8400)
					Globalv.base_offset_cent3 = float(value)
				"KnobAddedNoiseColor":
					var value:int = Globalv.noise_color_type+1
					value += direction * getAmount(1, 1)
					value  = clampi(value, 1, 2)
					Globalv.noise_color_type = value-1
				"KnobD1DelayTime":
					var value:int = int(Globalv.delay0_time)
					value += direction * getAmount(1, 10)
					value  = clampi(value, 0, 500)
					Globalv.delay0_time = float(value)
				"KnobD1Level":
					var value:float = Globalv.delay0_ratio
					value += float(direction) * getAmount(0.01, 0.1)
					value  = clampf(value, 0.0, 0.2)
					Globalv.delay0_ratio = value
				"KnobD2DelayTime":
					var value:int = int(Globalv.delay1_time)
					value += direction * getAmount(1, 10)
					value  = clampi(value, 0, 500)
					Globalv.delay1_time = float(value)
				"KnobD2Level":
					var value:float = Globalv.delay1_ratio
					value += float(direction) * getAmount(0.01, 0.1)
					value  = clampf(value, 0.0, 0.2)
					Globalv.delay1_ratio = value
				"KnobD3DelayTime":
					var value:int = int(Globalv.delay2_time)
					value += direction * getAmount(1, 10)
					value  = clampi(value, 0, 500)
					Globalv.delay2_time = float(value)
				"KnobD3Level":
					var value:float = Globalv.delay2_ratio
					value += float(direction) * getAmount(0.01, 0.1)
					value  = clampf(value, 0.0, 0.2)
					Globalv.delay2_ratio = value
				"KnobFDNDistributionType":
					var value:int = Globalv.freq_noise_type+1
					value += direction * getAmount(1, 1)
					value  = clampi(value, 1, 3)
					Globalv.freq_noise_type = value-1
				"KnobFDNCentRange":
					var value:int = int(Globalv.freq_noise_cent_range)
					value += direction * getAmount(1, 100)
					value  = clampi(value, 0, 8400)
					Globalv.freq_noise_cent_range = float(value)
				"KnobFMCarrierFreq":
					var value:int = int(Globalv.fm_freq)
					value += direction * getAmount(1, 10)
					value  = clampi(value, 0, 880)
					Globalv.fm_freq = float(value)
				"KnobFMCentRange":
					var value:int = int(Globalv.fm_cent_range)
					value += direction * getAmount(1, 100)
					value  = clampi(value, -2400, 8400)
					Globalv.fm_cent_range = float(value)
				"KnobFMPhaseOffset":
					var value:float = Globalv.fm_phase_offset*180.0
					value += direction * getAmount(1.0, 45.0)
					value  = clampf(value, 0.0, 360.0)
					Globalv.fm_phase_offset = float(value)/180.0
				"KnobFMCarrierWaveform":
					var value:int = Globalv.fm_wave+1
					value += direction * getAmount(1, 1)
					value  = clampi(value, 1, 5)
					Globalv.fm_wave = value-1
				"KnobFMSyncWithTempo":
					var value:int = -Globalv.fm_sync+2
					value += direction * getAmount(1, 1)
					value  = clampi(value, 1, 2)
					Globalv.fm_sync = -(value-2)
				"KnobAMCarrierFreq":
					var value:int = int(Globalv.am_freq)
					value += direction * getAmount(1, 10)
					value  = clampi(value, 0, 880)
					Globalv.am_freq = float(value)
				"KnobAMAmplitudeLevel":
					var value:float = Globalv.am_level
					value += float(direction) * getAmount(0.01, 0.1)
					value  = clampf(value, 0.0, 1.0)
					Globalv.am_level = value
				"KnobAMPhaseOffset":
					var value:float = Globalv.am_phase_offset*180.0
					value += direction * getAmount(1.0, 45.0)
					value  = clampf(value, 0.0, 360.0)
					Globalv.am_phase_offset = float(value)/180.0
				"KnobAMCarrierWaveform":
					var value:int = Globalv.am_wave+1
					value += direction * getAmount(1, 1)
					value  = clampi(value, 1, 5)
					Globalv.am_wave = value-1
				"KnobAMSyncWithTempo":
					var value:int = -Globalv.am_sync+2
					value += direction * getAmount(1, 1)
					value  = clampi(value, 1, 2)
					Globalv.am_sync = -(value-2)
				var n :
					print(n, " is not used")
					return

			knob_draw()
			$"../../GDSynthesizer".modify_synthe_params()
