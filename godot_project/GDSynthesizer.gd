extends GDSynthesizer

var synthe_params_array:Array
var percussion_params_array:Array

func copy_program_params()->void:
	var index:int = Globalv.program
	Globalv.total_gain            = synthe_params_array[index]["totalGain"]
	Globalv.atack_slope_time      = synthe_params_array[index]["atackSlopeTime"]
	Globalv.decay_half_life_time  = synthe_params_array[index]["decayHalfLifeTime"]
	Globalv.sustain_rate          = synthe_params_array[index]["sustainRate"]
	Globalv.release_slope_time    = synthe_params_array[index]["releaseSlopeTime"]
	Globalv.base_vs_others_ratio  = synthe_params_array[index]["baseVsOthersRatio"]
	Globalv.side1_vs_side2_ratio  = synthe_params_array[index]["side1VsSide2Ratio"]
	Globalv.base_offset_cent1     = synthe_params_array[index]["baseOffsetCent1"]
	Globalv.base_wave1            = synthe_params_array[index]["baseWave1"]
	Globalv.base_offset_cent2     = synthe_params_array[index]["baseOffsetCent2"]
	Globalv.base_wave2            = synthe_params_array[index]["baseWave2"]
	Globalv.base_offset_cent3     = synthe_params_array[index]["baseOffsetCent3"]
	Globalv.base_wave3            = synthe_params_array[index]["baseWave3"]
	Globalv.noise_ratio           = synthe_params_array[index]["noiseRatio"]
	Globalv.noise_color_type      = synthe_params_array[index]["noiseColorType"]
	Globalv.delay0_time           = synthe_params_array[index]["delay0Time"]
	Globalv.delay0_ratio          = synthe_params_array[index]["delay0Ratio"]
	Globalv.delay1_time           = synthe_params_array[index]["delay1Time"]
	Globalv.delay1_ratio          = synthe_params_array[index]["delay1Ratio"]
	Globalv.delay2_time           = synthe_params_array[index]["delay2Time"]
	Globalv.delay2_ratio          = synthe_params_array[index]["delay2Ratio"]
	Globalv.freq_noise_cent_range = synthe_params_array[index]["freqNoiseCentRange"]
	Globalv.freq_noise_type       = synthe_params_array[index]["freqNoiseType"]
	Globalv.fm_cent_range         = synthe_params_array[index]["fmCentRange"]
	Globalv.fm_freq               = synthe_params_array[index]["fmFreq"]
	Globalv.fm_phase_offset       = synthe_params_array[index]["fmPhaseOffset"]
	Globalv.fm_sync               = synthe_params_array[index]["fmSync"]
	Globalv.fm_wave               = synthe_params_array[index]["fmWave"]
	Globalv.am_level              = synthe_params_array[index]["amLevel"]
	Globalv.am_freq               = synthe_params_array[index]["amFreq"]
	Globalv.am_phase_offset       = synthe_params_array[index]["amPhaseOffset"]
	Globalv.am_sync               = synthe_params_array[index]["amSync"]
	Globalv.am_wave               = synthe_params_array[index]["amWave"]

	Globalv.is_loaded = true
	Globalv.num_execution = 0

func modify_program_params()->void:
	var index:int = Globalv.program
	synthe_params_array[index]["totalGain"]         = Globalv.total_gain            
	synthe_params_array[index]["atackSlopeTime"]    = Globalv.atack_slope_time      
	synthe_params_array[index]["decayHalfLifeTime"] = Globalv.decay_half_life_time  
	synthe_params_array[index]["sustainRate"]       = Globalv.sustain_rate          
	synthe_params_array[index]["releaseSlopeTime"]  = Globalv.release_slope_time    
	synthe_params_array[index]["baseVsOthersRatio"] = Globalv.base_vs_others_ratio  
	synthe_params_array[index]["side1VsSide2Ratio"] = Globalv.side1_vs_side2_ratio  
	synthe_params_array[index]["baseOffsetCent1"]   = Globalv.base_offset_cent1     
	synthe_params_array[index]["baseWave1"]         = Globalv.base_wave1            
	synthe_params_array[index]["baseOffsetCent2"]   = Globalv.base_offset_cent2     
	synthe_params_array[index]["baseWave2"]         = Globalv.base_wave2            
	synthe_params_array[index]["baseOffsetCent3"]   = Globalv.base_offset_cent3     
	synthe_params_array[index]["baseWave3"]         = Globalv.base_wave3            
	synthe_params_array[index]["noiseRatio"]        = Globalv.noise_ratio           
	synthe_params_array[index]["noiseColorType"]    = Globalv.noise_color_type      
	synthe_params_array[index]["delay0Time"]        = Globalv.delay0_time           
	synthe_params_array[index]["delay0Ratio"]       = Globalv.delay0_ratio          
	synthe_params_array[index]["delay1Time"]        = Globalv.delay1_time           
	synthe_params_array[index]["delay1Ratio"]       = Globalv.delay1_ratio          
	synthe_params_array[index]["delay2Time"]        = Globalv.delay2_time           
	synthe_params_array[index]["delay2Ratio"]       = Globalv.delay2_ratio          
	synthe_params_array[index]["freqNoiseCentRange"]= Globalv.freq_noise_cent_range 
	synthe_params_array[index]["freqNoiseType"]     = Globalv.freq_noise_type       
	synthe_params_array[index]["fmCentRange"]       = Globalv.fm_cent_range         
	synthe_params_array[index]["fmFreq"]            = Globalv.fm_freq               
	synthe_params_array[index]["fmPhaseOffset"]     = Globalv.fm_phase_offset       
	synthe_params_array[index]["fmSync"]            = Globalv.fm_sync               
	synthe_params_array[index]["fmWave"]            = Globalv.fm_wave               
	synthe_params_array[index]["amLevel"]           = Globalv.am_level              
	synthe_params_array[index]["amFreq"]            = Globalv.am_freq               
	synthe_params_array[index]["amPhaseOffset"]     = Globalv.am_phase_offset       
	synthe_params_array[index]["amSync"]            = Globalv.am_sync               
	synthe_params_array[index]["amWave"]            = Globalv.am_wave               


func copy_percussion_params()->void:
	if Globalv.percussion_key != -1:
		var index:int = Globalv.percussion_key
		Globalv.percussion_prog = percussion_params_array[index]["program"]
		Globalv.percussion_note = percussion_params_array[index]["key"]


func modify_percussion_params()->void:
	if Globalv.percussion_key != -1:
		var index:int = Globalv.percussion_key
		percussion_params_array[index]["program"]  = Globalv.percussion_prog
		percussion_params_array[index]["key"]      = Globalv.percussion_note

func get_mini_wave_image(type:int, phase:int, target)->void:
	var image_params = {
		"type": type,
		"size_x": 40,
		"size_y": 40,
		"phase": phase}
	var image := get_mini_wave_picture(image_params)
	var tex := ImageTexture.create_from_image(image)
	tex.update(image)
	target.set_texture(tex)


func _ready()->void:
	init_synthe(Globalv.num_poly);
	synthe_params_array = get_synthe_params()
	copy_program_params()
	var ctr_params = get_control_params()
	Globalv.num_division = ctr_params["divisionNum"]
	percussion_params_array = get_percussion_params()
	
	# Set preOnTime to 5 seconds (5000ms) for piano roll visualization
	ctr_params["preOnTime"] = 5000.0
	set_control_params(ctr_params)

	play(0.0)


func program_change()->void:
	copy_program_params()
	Globalv.is_loaded = true
	Globalv.num_execution = 0


func modify_synthe_params()->void:
	modify_program_params()
	set_synthe_params(synthe_params_array)
	set_percussion_params(percussion_params_array)
	
	# Get current control params to preserve preOnTime
	var current_ctr_params = get_control_params()
	var ctr_params = {
		"divisionNum": Globalv.num_division,
		"logLevel": Globalv.log_level,
		"preOnTime": current_ctr_params.get("preOnTime", 5000.0)  # Preserve preOnTime (default to 5000.0 if not set)
	}
	set_control_params(ctr_params)
	$"..".redraw_signal_allocation_bar()

func _process(delta:float)->void:
	feed_data(delta)


func _on_button_unselect_smf_button_up()->void:
	Globalv.smf_filename = ""
	$"..".info_change()
	unload_midi()
	Globalv.is_keyboard_clear = true 
	$"..".keyboard_clear_if_requied()


func _on_file_dialog_file_selected(path:StringName)->void:
	Globalv.smf_filename = path
	print(path)
	$"..".info_change()

	unload_midi()
	Globalv.is_keyboard_clear = true 
	$"..".keyboard_clear_if_requied()

	var res:int = load_midi(path)
	if res == 1:
		print("open success")
	else:
		print("open failure")


func _on_button_select_smf_button_up()->void:
	# note that https://github.com/godotengine/godot/issues/53339
	# "In exported games, a FileDialog in Resource mode cannot open 
	# resources in subfolders #53339"
	$"../ButtonSelectSMF/FileDialog".show()


func _on_button_load_data_button_up()->void:
	$"../ButtonLoadData/FileDialog".show()


func _on_button_save_data_button_up()->void:
	$"../ButtonSaveData/FileDialog".show()


func _on_file_dialog_load_file_selected(path:StringName)->void:
	var json_string:String	= FileAccess.get_file_as_string(path)
	var json:JSON = JSON.new()
	var parse_result:Error = json.parse(json_string)
	if parse_result == OK:
		print("parse OK")
		var loaddata:Variant = json.get_data()
		Globalv.num_poly        = loaddata["num_poly"]
		Globalv.program         = loaddata["program"]
		Globalv.base_tempo      = loaddata["base_tempo"]
		Globalv.is_percussion   = loaddata["is_percussion"]
		synthe_params_array     = loaddata["synthe_params_array"]
		percussion_params_array = loaddata["percussion__params_array"]

		$"..".reset_percussion_related()
		copy_program_params()
		set_synthe_params(synthe_params_array)
		set_percussion_params(percussion_params_array)


func _on_file_dialog_save_file_selected(path:StringName)->void:
	var savedata = {
		"num_poly":                 Globalv.num_poly,
		"program":                  Globalv.program,
		"base_tempo":               Globalv.base_tempo,
		"is_percussion":            Globalv.is_percussion,
		"synthe_params_array":      synthe_params_array,
		"percussion__params_array": percussion_params_array
	}
	var json_string:String = JSON.stringify(savedata, "\t")
	var f:FileAccess = FileAccess.open(path, FileAccess.WRITE)
	f.store_string(json_string)
	f.close()
