extends Control

# Piano Roll Overlay for displaying notes
# Displays notes for the currently selected PROGRAM as scrolling rectangles
# Notes flow from top to bottom, divided equally for each key

# Centralize constants - changes here will be reflected everywhere
const PIANO_ROLL_WIDTH: float = 1000.0

# Height options for right-click toggle: 350 -> 414 -> 447 -> 350 -> ...
const HEIGHT_OPTIONS: Array[float] = [350.0+65.0, 414.0+50.0, 447.0+50.0]
const PIANO_ROLL_HEIGHT: float = HEIGHT_OPTIONS[0]  # Initial height (default)

# Piano roll color constants - Change these to adjust all related colors
const COLOR_WHITE_KEY: Color = Color(0.6, 0.55, 0.55, 1.0)  # Gray for white keys
const COLOR_BLACK_KEY: Color = Color(0.0, 0.0, 0.0, 1.0)    # Black for black keys
const COLOR_GRID_VERTICAL: Color = Color(0.1, 0.2, 0.4, 0.8)  # Blue for vertical grid lines
const COLOR_GRID_HORIZONTAL: Color = Color(0.1, 0.2, 0.4, 0.5)  # Blue for horizontal grid lines
const COLOR_FALLBACK: Color = Color(1.0, 0.0, 0.0, 0.9)  # Red fallback color

var gd_synthesizer: Node = null
var front_case: Node = null  # Reference to FrontCase
var active_notes: Array[Dictionary] = []  # Array of {key: int, start_time: float, end_time: float, program: int, channel: int, note_height: float}
var pre_on_time: float = 5.0  # preOnTime in seconds (matches GDSynthesizer setting)
var current_height: float = PIANO_ROLL_HEIGHT  # Current piano roll height (can be changed by right-click)
var viewport_height: float = PIANO_ROLL_HEIGHT  # Piano roll viewport height in pixels (synced with current_height)
var height_index: int = 0  # Current index in HEIGHT_OPTIONS array
var scroll_speed: float = 0.0  # Pixels per second (calculated as viewport_height / pre_on_time)
var start_time_offset: float = -1.0  # Time offset for scrolling (-1 means not initialized yet)
var default_note_height: float = 0.0  # Default note height for 10 minutes (600 seconds)
var is_started: bool = false  # Whether playback has started (first note received)
var last_smf_filename: String = ""  # Track SMF filename changes to clear notes on load/unload
var show_all_programs: bool = false  # Display mode: false = single program, true = all programs
var program_color_map: Array[Color] = []  # Color map for 128 programs (indexed by program number)
var last_percussion_mode: bool = false  # Previous percussion mode state
var is_percussion_overlay: bool = false  # Whether this overlay is for percussion
var last_pre_on_time: float = -1.0  # Track preOnTime changes

func _ready():
	# Initialize height settings
	current_height = PIANO_ROLL_HEIGHT
	height_index = 0
	viewport_height = current_height
	
	# Find parent TextureRect and set its size dynamically
	var texture_rect = get_parent()  # PianoRoll/TextureRect
	if texture_rect and texture_rect is TextureRect:
		# Set anchors to fix top-left corner (prevents position shift when size changes) - step 5
		# These are constant values, so set them once in _ready()
		texture_rect.set_anchors_preset(Control.PRESET_TOP_LEFT)
		texture_rect.anchor_left = 0.0
		texture_rect.anchor_top = 0.0
		texture_rect.anchor_right = 0.0
		texture_rect.anchor_bottom = 0.0
		texture_rect.offset_left = 0.0
		texture_rect.offset_top = 0.0
		texture_rect.offset_right = PIANO_ROLL_WIDTH  # Constant value
		
		# Set TextureRect size using constants
		texture_rect.custom_minimum_size = Vector2(PIANO_ROLL_WIDTH, PIANO_ROLL_HEIGHT)
		texture_rect.size = Vector2(PIANO_ROLL_WIDTH, PIANO_ROLL_HEIGHT)
		texture_rect.offset_bottom = PIANO_ROLL_HEIGHT
	
	# Find GDSynthesizer node and FrontCase
	# Node structure: FrontCase -> PianoRoll -> TextureRect -> PianoRollOverlay
	var parent_node = get_parent().get_parent().get_parent()
	if parent_node:
		front_case = parent_node  # Save reference to FrontCase
		gd_synthesizer = parent_node.get_node_or_null("GDSynthesizer")
		if gd_synthesizer:
			# Wait for next frame to ensure GDSynthesizer._ready() has completed
			await get_tree().process_frame
			
			# Get preOnTime from GDSynthesizer
			var ctr_params = gd_synthesizer.get_control_params()
			if ctr_params.has("preOnTime"):
				pre_on_time = ctr_params["preOnTime"] / 1000.0  # Convert from milliseconds to seconds
			
			# Fallback if preOnTime is 0 or invalid
			if pre_on_time <= 0.0:
				pre_on_time = 5.0  # Default to 5 seconds
			
			# Calculate scroll speed: viewport height represents preOnTime
			scroll_speed = viewport_height / pre_on_time
			
			# Default note height for 10 minutes (600 seconds)
			default_note_height = 600.0 * scroll_speed
			
			# Connect to pre_note_changed signal
			if not gd_synthesizer.pre_note_changed.is_connected(_on_pre_note_changed):
				gd_synthesizer.pre_note_changed.connect(_on_pre_note_changed)
			
		else:
			pass
	else:
		pass
	
	# Set up drawing
	set_process(true)
	# start_time_offset will be set when first pre_note_on is received
	
	# Ensure Control can receive mouse events (left-click for height toggle)
	mouse_filter = Control.MOUSE_FILTER_STOP  # Stop mouse events to handle left-click
	clip_contents = true  # Clip drawing to this node's rectangle
	
	# Initialize scroll speed and default note height based on initial height
	if pre_on_time > 0.0:
		scroll_speed = viewport_height / pre_on_time
		default_note_height = 600.0 * scroll_speed
	
	# Generate color map for programs
	program_color_map = _generate_color_map()
	
	# Determine role by node name
	var node_name = name
	if node_name == "PianoRollOverlayPercussion":
		is_percussion_overlay = true
	elif node_name == "PianoRollOverlayNonPercussion":
		is_percussion_overlay = false
	else:
		# Use get_is_percussion_mode() for compatibility with existing PianoRollOverlay
		is_percussion_overlay = get_is_percussion_mode()
	
	# Force initial redraw
	queue_redraw()

var last_program: int = -1

func get_is_percussion_mode() -> bool:
	# Determine by node name (for compatibility with existing code)
	# New implementation uses is_percussion_overlay
	return is_percussion_overlay

func _generate_color_map() -> Array[Color]:
	# Fixed color table with 128 colors that satisfy:
	# 1. 1.0 <= R+G+B <= 3.0 (brightness not too low)
	# 2. At least one RGB component differs by 0.5+ from others (saturation)
	# 3. Colors are sufficiently different from each other
	
	var colors: Array[Color] = []
	
	# Red-dominant colors (R high, G/B low) - 43 colors
	var r_dominant = [
		Color(0.5, 0.0, 0.5, 0.9),    # R+G+B=1.0, diff=0.5
		Color(0.6, 0.0, 0.4, 0.9),    # R+G+B=1.0, diff=0.6
		Color(0.7, 0.0, 0.3, 0.9),    # R+G+B=1.0, diff=0.7
		Color(0.8, 0.0, 0.2, 0.9),    # R+G+B=1.0, diff=0.8
		Color(0.9, 0.0, 0.1, 0.9),    # R+G+B=1.0, diff=0.9
		Color(1.0, 0.0, 0.0, 0.9),    # R+G+B=1.0, diff=1.0
		Color(0.5, 0.1, 0.4, 0.9),    # R+G+B=1.0, diff=0.5
		Color(0.6, 0.1, 0.3, 0.9),    # R+G+B=1.0, diff=0.6
		Color(0.7, 0.1, 0.2, 0.9),    # R+G+B=1.0, diff=0.7
		Color(0.8, 0.1, 0.1, 0.9),    # R+G+B=1.0, diff=0.8
		Color(0.9, 0.1, 0.0, 0.9),    # R+G+B=1.0, diff=0.9
		Color(1.0, 0.1, 0.0, 0.9),    # R+G+B=1.1, diff=1.0
		Color(0.5, 0.2, 0.3, 0.9),    # R+G+B=1.0, diff=0.5
		Color(0.6, 0.2, 0.2, 0.9),    # R+G+B=1.0, diff=0.6
		Color(0.7, 0.2, 0.1, 0.9),    # R+G+B=1.0, diff=0.7
		Color(0.8, 0.2, 0.0, 0.9),    # R+G+B=1.0, diff=0.8
		Color(0.9, 0.2, 0.0, 0.9),    # R+G+B=1.1, diff=0.9
		Color(1.0, 0.2, 0.0, 0.9),    # R+G+B=1.2, diff=1.0
		Color(0.5, 0.3, 0.2, 0.9),    # R+G+B=1.0, diff=0.5
		Color(0.6, 0.3, 0.1, 0.9),    # R+G+B=1.0, diff=0.6
		Color(0.7, 0.3, 0.0, 0.9),    # R+G+B=1.0, diff=0.7
		Color(0.8, 0.3, 0.0, 0.9),    # R+G+B=1.1, diff=0.8
		Color(0.9, 0.3, 0.0, 0.9),    # R+G+B=1.2, diff=0.9
		Color(1.0, 0.3, 0.0, 0.9),    # R+G+B=1.3, diff=1.0
		Color(0.5, 0.4, 0.1, 0.9),    # R+G+B=1.0, diff=0.5
		Color(0.6, 0.4, 0.0, 0.9),    # R+G+B=1.0, diff=0.6
		Color(0.7, 0.4, 0.0, 0.9),    # R+G+B=1.1, diff=0.7
		Color(0.8, 0.4, 0.0, 0.9),    # R+G+B=1.2, diff=0.8
		Color(0.9, 0.4, 0.0, 0.9),    # R+G+B=1.3, diff=0.9
		Color(1.0, 0.4, 0.0, 0.9),    # R+G+B=1.4, diff=1.0
		Color(0.6, 0.0, 0.5, 0.9),    # R+G+B=1.1, diff=0.6
		Color(0.7, 0.0, 0.4, 0.9),    # R+G+B=1.1, diff=0.7
		Color(0.8, 0.0, 0.3, 0.9),    # R+G+B=1.1, diff=0.8
		Color(0.9, 0.0, 0.2, 0.9),    # R+G+B=1.1, diff=0.9
		Color(1.0, 0.0, 0.1, 0.9),    # R+G+B=1.1, diff=1.0
		Color(0.7, 0.0, 0.5, 0.9),    # R+G+B=1.2, diff=0.7
		Color(0.8, 0.0, 0.4, 0.9),    # R+G+B=1.2, diff=0.8
		Color(0.9, 0.0, 0.3, 0.9),    # R+G+B=1.2, diff=0.9
		Color(1.0, 0.0, 0.2, 0.9),    # R+G+B=1.2, diff=1.0
		Color(0.8, 0.0, 0.5, 0.9),    # R+G+B=1.3, diff=0.8
		Color(0.9, 0.0, 0.4, 0.9),    # R+G+B=1.3, diff=0.9
		Color(1.0, 0.0, 0.3, 0.9),    # R+G+B=1.3, diff=1.0
		Color(0.9, 0.0, 0.5, 0.9),    # R+G+B=1.4, diff=0.9
		Color(1.0, 0.0, 0.4, 0.9),    # R+G+B=1.4, diff=1.0
	]
	colors.append_array(r_dominant)
	
	# Green-dominant colors (G high, R/B low) - 43 colors
	var g_dominant = [
		Color(0.0, 0.5, 0.5, 0.9),    # R+G+B=1.0, diff=0.5
		Color(0.0, 0.6, 0.4, 0.9),    # R+G+B=1.0, diff=0.6
		Color(0.0, 0.7, 0.3, 0.9),    # R+G+B=1.0, diff=0.7
		Color(0.0, 0.8, 0.2, 0.9),    # R+G+B=1.0, diff=0.8
		Color(0.0, 0.9, 0.1, 0.9),    # R+G+B=1.0, diff=0.9
		Color(0.0, 1.0, 0.0, 0.9),    # R+G+B=1.0, diff=1.0
		Color(0.1, 0.5, 0.4, 0.9),    # R+G+B=1.0, diff=0.5
		Color(0.1, 0.6, 0.3, 0.9),    # R+G+B=1.0, diff=0.6
		Color(0.1, 0.7, 0.2, 0.9),    # R+G+B=1.0, diff=0.7
		Color(0.1, 0.8, 0.1, 0.9),    # R+G+B=1.0, diff=0.8
		Color(0.1, 0.9, 0.0, 0.9),    # R+G+B=1.0, diff=0.9
		Color(0.1, 1.0, 0.0, 0.9),    # R+G+B=1.1, diff=1.0
		Color(0.2, 0.5, 0.3, 0.9),    # R+G+B=1.0, diff=0.5
		Color(0.2, 0.6, 0.2, 0.9),    # R+G+B=1.0, diff=0.6
		Color(0.2, 0.7, 0.1, 0.9),    # R+G+B=1.0, diff=0.7
		Color(0.2, 0.8, 0.0, 0.9),    # R+G+B=1.0, diff=0.8
		Color(0.2, 0.9, 0.0, 0.9),    # R+G+B=1.1, diff=0.9
		Color(0.2, 1.0, 0.0, 0.9),    # R+G+B=1.2, diff=1.0
		Color(0.3, 0.5, 0.2, 0.9),    # R+G+B=1.0, diff=0.5
		Color(0.3, 0.6, 0.1, 0.9),    # R+G+B=1.0, diff=0.6
		Color(0.3, 0.7, 0.0, 0.9),    # R+G+B=1.0, diff=0.7
		Color(0.3, 0.8, 0.0, 0.9),    # R+G+B=1.1, diff=0.8
		Color(0.3, 0.9, 0.0, 0.9),    # R+G+B=1.2, diff=0.9
		Color(0.3, 1.0, 0.0, 0.9),    # R+G+B=1.3, diff=1.0
		Color(0.4, 0.5, 0.1, 0.9),    # R+G+B=1.0, diff=0.5
		Color(0.4, 0.6, 0.0, 0.9),    # R+G+B=1.0, diff=0.6
		Color(0.4, 0.7, 0.0, 0.9),    # R+G+B=1.1, diff=0.7
		Color(0.4, 0.8, 0.0, 0.9),    # R+G+B=1.2, diff=0.8
		Color(0.4, 0.9, 0.0, 0.9),    # R+G+B=1.3, diff=0.9
		Color(0.4, 1.0, 0.0, 0.9),    # R+G+B=1.4, diff=1.0
		Color(0.0, 0.6, 0.5, 0.9),    # R+G+B=1.1, diff=0.6
		Color(0.0, 0.7, 0.4, 0.9),    # R+G+B=1.1, diff=0.7
		Color(0.0, 0.8, 0.3, 0.9),    # R+G+B=1.1, diff=0.8
		Color(0.0, 0.9, 0.2, 0.9),    # R+G+B=1.1, diff=0.9
		Color(0.0, 1.0, 0.1, 0.9),    # R+G+B=1.1, diff=1.0
		Color(0.0, 0.7, 0.5, 0.9),    # R+G+B=1.2, diff=0.7
		Color(0.0, 0.8, 0.4, 0.9),    # R+G+B=1.2, diff=0.8
		Color(0.0, 0.9, 0.3, 0.9),    # R+G+B=1.2, diff=0.9
		Color(0.0, 1.0, 0.2, 0.9),    # R+G+B=1.2, diff=1.0
		Color(0.0, 0.8, 0.5, 0.9),    # R+G+B=1.3, diff=0.8
		Color(0.0, 0.9, 0.4, 0.9),    # R+G+B=1.3, diff=0.9
		Color(0.0, 1.0, 0.3, 0.9),    # R+G+B=1.3, diff=1.0
		Color(0.0, 0.9, 0.5, 0.9),    # R+G+B=1.4, diff=0.9
		Color(0.0, 1.0, 0.4, 0.9),    # R+G+B=1.4, diff=1.0
	]
	colors.append_array(g_dominant)
	
	# Blue-dominant colors (B high, R/G low) - 42 colors
	var b_dominant = [
		Color(0.4, 0.0, 0.6, 0.9),    # R+G+B=1.0, diff=0.6
		Color(0.3, 0.0, 0.7, 0.9),    # R+G+B=1.0, diff=0.7
		Color(0.2, 0.0, 0.8, 0.9),    # R+G+B=1.0, diff=0.8
		Color(0.1, 0.0, 0.9, 0.9),    # R+G+B=1.0, diff=0.9
		Color(0.0, 0.0, 1.0, 0.9),    # R+G+B=1.0, diff=1.0
		Color(0.4, 0.1, 0.5, 0.9),    # R+G+B=1.0, diff=0.5
		Color(0.3, 0.1, 0.6, 0.9),    # R+G+B=1.0, diff=0.6
		Color(0.2, 0.1, 0.7, 0.9),    # R+G+B=1.0, diff=0.7
		Color(0.1, 0.1, 0.8, 0.9),    # R+G+B=1.0, diff=0.8
		Color(0.0, 0.1, 0.9, 0.9),    # R+G+B=1.0, diff=0.9
		Color(0.0, 0.1, 1.0, 0.9),    # R+G+B=1.1, diff=1.0
		Color(0.3, 0.2, 0.5, 0.9),    # R+G+B=1.0, diff=0.5
		Color(0.2, 0.2, 0.6, 0.9),    # R+G+B=1.0, diff=0.6
		Color(0.1, 0.2, 0.7, 0.9),    # R+G+B=1.0, diff=0.7
		Color(0.0, 0.2, 0.8, 0.9),    # R+G+B=1.0, diff=0.8
		Color(0.0, 0.2, 0.9, 0.9),    # R+G+B=1.1, diff=0.9
		Color(0.0, 0.2, 1.0, 0.9),    # R+G+B=1.2, diff=1.0
		Color(0.2, 0.3, 0.5, 0.9),    # R+G+B=1.0, diff=0.5
		Color(0.1, 0.3, 0.6, 0.9),    # R+G+B=1.0, diff=0.6
		Color(0.0, 0.3, 0.7, 0.9),    # R+G+B=1.0, diff=0.7
		Color(0.0, 0.3, 0.8, 0.9),    # R+G+B=1.1, diff=0.8
		Color(0.0, 0.3, 0.9, 0.9),    # R+G+B=1.2, diff=0.9
		Color(0.0, 0.3, 1.0, 0.9),    # R+G+B=1.3, diff=1.0
		Color(0.1, 0.4, 0.5, 0.9),    # R+G+B=1.0, diff=0.5
		Color(0.0, 0.4, 0.6, 0.9),    # R+G+B=1.0, diff=0.6
		Color(0.0, 0.4, 0.7, 0.9),    # R+G+B=1.1, diff=0.7
		Color(0.0, 0.4, 0.8, 0.9),    # R+G+B=1.2, diff=0.8
		Color(0.0, 0.4, 0.9, 0.9),    # R+G+B=1.3, diff=0.9
		Color(0.0, 0.4, 1.0, 0.9),    # R+G+B=1.4, diff=1.0
		Color(0.0, 0.5, 0.6, 0.9),    # R+G+B=1.1, diff=0.6
		Color(0.0, 0.5, 0.7, 0.9),    # R+G+B=1.2, diff=0.7
		Color(0.0, 0.5, 0.8, 0.9),    # R+G+B=1.3, diff=0.8
		Color(0.0, 0.5, 0.9, 0.9),    # R+G+B=1.4, diff=0.9
		Color(0.0, 0.5, 1.0, 0.9),    # R+G+B=1.5, diff=1.0
		Color(0.5, 0.0, 0.6, 0.9),    # R+G+B=1.1, diff=0.6
		Color(0.4, 0.0, 0.7, 0.9),    # R+G+B=1.1, diff=0.7
		Color(0.3, 0.0, 0.8, 0.9),    # R+G+B=1.1, diff=0.8
		Color(0.2, 0.0, 0.9, 0.9),    # R+G+B=1.1, diff=0.9
		Color(0.1, 0.0, 1.0, 0.9),    # R+G+B=1.1, diff=1.0
		Color(0.5, 0.0, 0.7, 0.9),    # R+G+B=1.2, diff=0.7
		Color(0.4, 0.0, 0.8, 0.9),    # R+G+B=1.2, diff=0.8
		Color(0.3, 0.0, 0.9, 0.9),    # R+G+B=1.2, diff=0.9
		Color(0.2, 0.0, 1.0, 0.9),    # R+G+B=1.2, diff=1.0
	]
	colors.append_array(b_dominant)
	
	# Mixed colors (two components high, one low) - 42 colors
	# R+G high, B low
	var mixed_rg = [
		Color(0.6, 0.4, 0.0, 0.9),    # R+G+B=1.0, diff=0.6
		Color(0.7, 0.3, 0.0, 0.9),    # R+G+B=1.0, diff=0.7
		Color(0.8, 0.2, 0.0, 0.9),    # R+G+B=1.0, diff=0.8
		Color(0.9, 0.1, 0.0, 0.9),    # R+G+B=1.0, diff=0.9
		Color(0.4, 0.6, 0.0, 0.9),    # R+G+B=1.0, diff=0.6
		Color(0.3, 0.7, 0.0, 0.9),    # R+G+B=1.0, diff=0.7
		Color(0.2, 0.8, 0.0, 0.9),    # R+G+B=1.0, diff=0.8
		Color(0.1, 0.9, 0.0, 0.9),    # R+G+B=1.0, diff=0.9
		Color(0.6, 0.5, 0.0, 0.9),    # R+G+B=1.1, diff=0.6
		Color(0.7, 0.4, 0.0, 0.9),    # R+G+B=1.1, diff=0.7
		Color(0.8, 0.3, 0.0, 0.9),    # R+G+B=1.1, diff=0.8
		Color(0.9, 0.2, 0.0, 0.9),    # R+G+B=1.1, diff=0.9
		Color(0.5, 0.6, 0.0, 0.9),    # R+G+B=1.1, diff=0.6
		Color(0.4, 0.7, 0.0, 0.9),    # R+G+B=1.1, diff=0.7
		Color(0.3, 0.8, 0.0, 0.9),    # R+G+B=1.1, diff=0.8
		Color(0.2, 0.9, 0.0, 0.9),    # R+G+B=1.1, diff=0.9
		Color(0.7, 0.5, 0.0, 0.9),    # R+G+B=1.2, diff=0.7
		Color(0.8, 0.4, 0.0, 0.9),    # R+G+B=1.2, diff=0.8
		Color(0.9, 0.3, 0.0, 0.9),    # R+G+B=1.2, diff=0.9
		Color(0.6, 0.6, 0.0, 0.9),    # R+G+B=1.2, diff=0.6
		Color(0.7, 0.6, 0.0, 0.9),    # R+G+B=1.3, diff=0.7
		Color(0.8, 0.6, 0.0, 0.9),    # R+G+B=1.4, diff=0.8
		Color(0.9, 0.6, 0.0, 0.9),    # R+G+B=1.5, diff=0.9
		Color(0.6, 0.7, 0.0, 0.9),    # R+G+B=1.3, diff=0.7
		Color(0.7, 0.7, 0.0, 0.9),    # R+G+B=1.4, diff=0.7
		Color(0.8, 0.7, 0.0, 0.9),    # R+G+B=1.5, diff=0.8
		Color(0.9, 0.7, 0.0, 0.9),    # R+G+B=1.6, diff=0.9
		Color(0.6, 0.8, 0.0, 0.9),    # R+G+B=1.4, diff=0.8
		Color(0.7, 0.8, 0.0, 0.9),    # R+G+B=1.5, diff=0.8
		Color(0.8, 0.8, 0.0, 0.9),    # R+G+B=1.6, diff=0.8
		Color(0.9, 0.8, 0.0, 0.9),    # R+G+B=1.7, diff=0.9
		Color(0.6, 0.9, 0.0, 0.9),    # R+G+B=1.5, diff=0.9
		Color(0.7, 0.9, 0.0, 0.9),    # R+G+B=1.6, diff=0.9
		Color(0.8, 0.9, 0.0, 0.9),    # R+G+B=1.7, diff=0.9
		Color(0.9, 0.9, 0.0, 0.9),    # R+G+B=1.8, diff=0.9
		Color(0.6, 1.0, 0.0, 0.9),    # R+G+B=1.6, diff=1.0
		Color(0.7, 1.0, 0.0, 0.9),    # R+G+B=1.7, diff=1.0
		Color(0.8, 1.0, 0.0, 0.9),    # R+G+B=1.8, diff=1.0
		Color(0.9, 1.0, 0.0, 0.9),    # R+G+B=1.9, diff=1.0
		Color(1.0, 1.0, 0.0, 0.9),    # R+G+B=2.0, diff=1.0
	]
	colors.append_array(mixed_rg)
	
	# R+B high, G low (avoiding duplicates with r_dominant)
	var mixed_rb = [
		Color(0.6, 0.0, 0.6, 0.9),    # R+G+B=1.2, diff=0.6
		Color(0.7, 0.0, 0.7, 0.9),    # R+G+B=1.4, diff=0.7
		Color(0.8, 0.0, 0.8, 0.9),    # R+G+B=1.6, diff=0.8
		Color(0.9, 0.0, 0.9, 0.9),    # R+G+B=1.8, diff=0.9
		Color(1.0, 0.0, 1.0, 0.9),    # R+G+B=2.0, diff=1.0
		Color(0.6, 0.1, 0.5, 0.9),    # R+G+B=1.2, diff=0.6
		Color(0.7, 0.1, 0.6, 0.9),    # R+G+B=1.4, diff=0.7
		Color(0.8, 0.1, 0.7, 0.9),    # R+G+B=1.6, diff=0.8
		Color(0.9, 0.1, 0.8, 0.9),    # R+G+B=1.8, diff=0.9
		Color(1.0, 0.1, 0.9, 0.9),    # R+G+B=2.0, diff=1.0
		Color(0.5, 0.2, 0.5, 0.9),    # R+G+B=1.2, diff=0.5
		Color(0.6, 0.2, 0.4, 0.9),    # R+G+B=1.2, diff=0.6
		Color(0.7, 0.2, 0.3, 0.9),    # R+G+B=1.2, diff=0.7
		Color(0.8, 0.2, 0.2, 0.9),    # R+G+B=1.2, diff=0.8
		Color(0.9, 0.2, 0.1, 0.9),    # R+G+B=1.2, diff=0.9
		Color(0.5, 0.3, 0.4, 0.9),    # R+G+B=1.2, diff=0.5
		Color(0.6, 0.3, 0.3, 0.9),    # R+G+B=1.2, diff=0.6
		Color(0.7, 0.3, 0.2, 0.9),    # R+G+B=1.2, diff=0.7
		Color(0.8, 0.3, 0.1, 0.9),    # R+G+B=1.2, diff=0.8
		Color(0.5, 0.4, 0.3, 0.9),    # R+G+B=1.2, diff=0.5
		Color(0.6, 0.4, 0.2, 0.9),    # R+G+B=1.2, diff=0.6
		Color(0.7, 0.4, 0.1, 0.9),    # R+G+B=1.2, diff=0.7
	]
	# Filter out duplicates
	for c in mixed_rb:
		var is_duplicate = false
		for existing in colors:
			if existing.r == c.r and existing.g == c.g and existing.b == c.b:
				is_duplicate = true
				break
		if not is_duplicate:
			colors.append(c)
	
	# G+B high, R low (avoiding duplicates with g_dominant and b_dominant)
	var mixed_gb = [
		Color(0.0, 0.6, 0.6, 0.9),    # R+G+B=1.2, diff=0.6
		Color(0.0, 0.7, 0.7, 0.9),    # R+G+B=1.4, diff=0.7
		Color(0.0, 0.8, 0.8, 0.9),    # R+G+B=1.6, diff=0.8
		Color(0.0, 0.9, 0.9, 0.9),    # R+G+B=1.8, diff=0.9
		Color(0.0, 1.0, 1.0, 0.9),    # R+G+B=2.0, diff=1.0
		Color(0.1, 0.6, 0.5, 0.9),    # R+G+B=1.2, diff=0.6
		Color(0.1, 0.7, 0.6, 0.9),    # R+G+B=1.4, diff=0.7
		Color(0.1, 0.8, 0.7, 0.9),    # R+G+B=1.6, diff=0.8
		Color(0.1, 0.9, 0.8, 0.9),    # R+G+B=1.8, diff=0.9
		Color(0.1, 1.0, 0.9, 0.9),    # R+G+B=2.0, diff=1.0
		Color(0.2, 0.6, 0.4, 0.9),    # R+G+B=1.2, diff=0.6
		Color(0.2, 0.7, 0.3, 0.9),    # R+G+B=1.2, diff=0.7
		Color(0.2, 0.8, 0.2, 0.9),    # R+G+B=1.2, diff=0.8
		Color(0.2, 0.9, 0.1, 0.9),    # R+G+B=1.2, diff=0.9
		Color(0.3, 0.6, 0.3, 0.9),    # R+G+B=1.2, diff=0.6
		Color(0.3, 0.7, 0.2, 0.9),    # R+G+B=1.2, diff=0.7
		Color(0.3, 0.8, 0.1, 0.9),    # R+G+B=1.2, diff=0.8
		Color(0.4, 0.6, 0.2, 0.9),    # R+G+B=1.2, diff=0.6
		Color(0.4, 0.7, 0.1, 0.9),    # R+G+B=1.2, diff=0.7
	]
	# Filter out duplicates
	for c in mixed_gb:
		var is_duplicate = false
		for existing in colors:
			if existing.r == c.r and existing.g == c.g and existing.b == c.b:
				is_duplicate = true
				break
		if not is_duplicate:
			colors.append(c)
	
	# Ensure we have exactly 128 colors
	# If we have less, add more mixed colors
	# If we have more, trim to 128
	while colors.size() < 128:
		# Add more high-brightness mixed colors
		for r in range(5, 11):  # 0.5 to 1.0
			for g in range(5, 11):
				for b in range(0, 5):  # 0.0 to 0.4
					var r_val = r * 0.1
					var g_val = g * 0.1
					var b_val = b * 0.1
					var sum = r_val + g_val + b_val
					if sum >= 1.0 and sum <= 3.0:
						var max_val = max(r_val, g_val, b_val)
						var min_val = min(r_val, g_val, b_val)
						if max_val - min_val >= 0.5:
							var c = Color(r_val, g_val, b_val, 0.9)
							var is_duplicate = false
							for existing in colors:
								if existing.r == c.r and existing.g == c.g and existing.b == c.b:
									is_duplicate = true
									break
							if not is_duplicate:
								colors.append(c)
								if colors.size() >= 128:
									break
				if colors.size() >= 128:
					break
			if colors.size() >= 128:
				break
		if colors.size() < 128:
			# If still not enough, add more variations
			break
	
	# Trim to exactly 128 if we have more
	if colors.size() > 128:
		colors = colors.slice(0, 128)
	
	return colors

func _gui_input(event: InputEvent):
	# Handle left-click to toggle piano roll height
	# Handle right-click to toggle display mode (single program / all programs)
	if event is InputEventMouseButton:
		var mouse_event = event as InputEventMouseButton
		if mouse_event.button_index == MOUSE_BUTTON_LEFT and mouse_event.pressed:
			_switch_height()
			# Accept the event to prevent it from propagating
			accept_event()
		elif mouse_event.button_index == MOUSE_BUTTON_RIGHT and mouse_event.pressed:
			_toggle_display_mode()
			# Accept the event to prevent it from propagating
			accept_event()

func _switch_height():
	# Cycle through height options: 350 -> 414 -> 447 -> 350 -> ... (step 3)
	height_index = (height_index + 1) % HEIGHT_OPTIONS.size()
	current_height = HEIGHT_OPTIONS[height_index]
	viewport_height = current_height
	
	# Recalculate scroll speed based on new height (step 3)
	if pre_on_time > 0.0:
		scroll_speed = viewport_height / pre_on_time
		default_note_height = 600.0 * scroll_speed
	
	# Update parent TextureRect size (step 4)
	var texture_rect = get_parent()  # PianoRoll/TextureRect
	if texture_rect and texture_rect is TextureRect:
		# Only update size-related properties that depend on current_height
		# (anchors and offset_left/offset_top/offset_right are set once in _ready())
		texture_rect.offset_bottom = current_height
		texture_rect.custom_minimum_size = Vector2(PIANO_ROLL_WIDTH, current_height)
		texture_rect.size = Vector2(PIANO_ROLL_WIDTH, current_height)
	
	# Update background mask size in FrontCase
	# Node structure: FrontCase -> PianoRoll -> TextureRect -> PianoRollOverlay
	var front_case = get_parent().get_parent().get_parent()  # FrontCase
	if front_case and front_case.has_method("update_piano_roll_background_size"):
		front_case.update_piano_roll_background_size()
	
	# PianoRollOverlay size will automatically follow parent TextureRect size
	# (due to PRESET_FULL_RECT anchors), so no need to set it explicitly
	
	# Force redraw
	queue_redraw()

func _toggle_display_mode():
	# Disable show_all_programs toggle for percussion overlay
	if is_percussion_overlay:
		# Percussion overlay always shows all, so do nothing
		return
	
	show_all_programs = not show_all_programs
	
	var current_program = Globalv.program
	for n in active_notes:
		if show_all_programs:
			n["visible"] = true
		else:
			n["visible"] = (n.get("program", -1) == current_program)
	
	# Notify FrontCase to update button state
	if front_case and front_case.has_method("update_allprograms_button_state"):
		front_case.update_allprograms_button_state(show_all_programs)
	
	queue_redraw()

func get_show_all_programs() -> bool:
	return show_all_programs

func _on_pre_note_changed(state: String, note: Dictionary):
	var channel = note.get("channel", -1)
	var is_percussion_channel = (channel == 9 or channel == 25)
	
	# Filter based on this overlay's role
	if is_percussion_overlay:
		if not is_percussion_channel:
			return
	else:
		if is_percussion_channel:
			return
	
	# For pre_note_changed signals, the key is stored as "key2"
	var key = note.get("key2", -1)
	if key < 0:
		# Fallback to "key" if "key2" doesn't exist
		key = note.get("key", -1)
		if key < 0:
			return
	
	# Check if key is within keyboard range
	if key < Globalv.most_left_key_num or key >= Globalv.most_left_key_num + Globalv.num_keyboard_key:
		return
	
	# Process notes for all PROGRAMS (not just the currently selected one)
	var current_program = Globalv.program
	var note_program = -1
	
	# Get program value for percussion note
	if is_percussion_overlay and is_percussion_channel:
		# Percussion overlay: get from percussion_params_array
		if gd_synthesizer:
			# Direct access to percussion_params_array (defined in GDSynthesizer.gd)
			var percussion_array = gd_synthesizer.percussion_params_array
			if percussion_array != null and percussion_array.size() > key:
				var percussion_item = percussion_array[key]
				if percussion_item != null and percussion_item.has("program"):
					note_program = percussion_item["program"]
				else:
					# Fallback: use normal instrumentNum
					note_program = note.get("instrumentNum", -1)
			else:
				# Fallback: use normal instrumentNum
				note_program = note.get("instrumentNum", -1)
		else:
			# Fallback: use normal instrumentNum
			note_program = note.get("instrumentNum", -1)
	else:
		# Non-percussion overlay: use conventional method
		note_program = note.get("instrumentNum", -1)
	
	var track_num = note.get("trackNum", -1)
	var current_time = Time.get_ticks_msec() / 1000.0
	var current_time_ms = Time.get_ticks_msec()
	
	# Initialize start_time_offset on first pre_note_on
	if not is_started and state == "pre_note_on":
		start_time_offset = current_time
		is_started = true
	
	# Skip if not started yet
	if start_time_offset < 0:
		return
	
	var relative_time = current_time - start_time_offset
	
	if state == "pre_note_on":
		# Create note rectangle with bottom edge at y=0 (top of viewport)
		# The rectangle will scroll down over 5 seconds (pre_on_time)
		var note_start_time = current_time - start_time_offset
		
		var note_visible: bool
		if is_percussion_overlay:
			note_visible = true
		else:
			note_visible = show_all_programs or (note_program == current_program)
		
		var note_data = {
			"key": key,
			"start_time": note_start_time,  # Time when pre_note_on was received (relative to start_time_offset)
			"end_time": -1.0,  # Will be set when pre_note_off is received
			"program": note_program,
			"channel": channel,
			"note_height": default_note_height,  # Initial height: 10 minutes worth (will be adjusted on pre_note_off)
			"visible": note_visible,  # Visible based on display mode
			"is_percussion": is_percussion_channel  # Record whether this is a percussion note
		}
		active_notes.append(note_data)
		# pre_note_on handled
	elif state == "pre_note_off":
		# Find and close the most recent matching open note (LIFO: last match from the end)
		# This handles the case where program changes between note_on and note_off
		# Match by key and channel only (ignore program to handle program changes)
		var matched_index = -1
		var matched_start_time = -1.0
		
		# Search from the end to find the most recent matching note
		for i in range(active_notes.size() - 1, -1, -1):
			var n = active_notes[i]
			# Match by key and channel only (ignore program to handle program changes)
			if n["key"] == key and n["channel"] == channel and n["end_time"] < 0:
				# Found a matching open note
				if matched_index < 0 or n["start_time"] > matched_start_time:
					# This is the most recent one
					matched_index = i
					matched_start_time = n["start_time"]
		
		if matched_index >= 0:
			# Close the most recent matching note
			var n = active_notes[matched_index]
			var elapsed_at_off = current_time - start_time_offset - n["start_time"]
			n["end_time"] = current_time - start_time_offset
			n["elapsed_at_off"] = elapsed_at_off
			# pre_note_off handled

func clear_all_notes()->void:
	# Clear all notes and reset state (called when SMF file is loaded/reloaded)
	active_notes.clear()
	is_started = false
	start_time_offset = -1.0
	queue_redraw()

func _update_pre_on_time(new_pre_on_time: float):
	var old_scroll_speed = scroll_speed
	
	# Update pre_on_time
	pre_on_time = new_pre_on_time
	
	# Recalculate scroll_speed
	if pre_on_time > 0.0:
		scroll_speed = viewport_height / pre_on_time
		default_note_height = 600.0 * scroll_speed
		
		# Recalculate note_height for existing notes
		if old_scroll_speed > 0.0 and scroll_speed > 0.0:
			var ratio = scroll_speed / old_scroll_speed
			for note in active_notes:
				if note.has("note_height"):
					note["note_height"] = note["note_height"] * ratio
	else:
		scroll_speed = 0.0
		default_note_height = 0.0
	
	queue_redraw()

func _process(delta):
	# Check for MIDI file load/unload and clear notes
	var current_smf_filename = Globalv.smf_filename
	if current_smf_filename != last_smf_filename:
		# MIDI file was loaded, reloaded, or unloaded - clear all notes
		clear_all_notes()
		last_smf_filename = current_smf_filename
	
	# Check for preOnTime changes
	if gd_synthesizer:
		var ctr_params = gd_synthesizer.get_control_params()
		if ctr_params.has("preOnTime"):
			var current_pre_on_time = ctr_params["preOnTime"] / 1000.0  # Convert to seconds
			if last_pre_on_time >= 0.0 and abs(current_pre_on_time - last_pre_on_time) > 0.001:
				# preOnTime has changed - update scroll speed and recalculate
				_update_pre_on_time(current_pre_on_time)
			last_pre_on_time = current_pre_on_time
	
	queue_redraw()

func _get_program_color(program: int) -> Color:
	# Color logic based on this overlay's role
	if is_percussion_overlay:
		# Percussion mode:
		# program 1-128 (0-127): yellow
		# program 129-256 (128-255): use program - 128 as color index
		if program >= 0 and program < 128:
			return Color(1.0, 1.0, 0, 1)
		elif program >= 128 and program < 256:
			var color_index = program - 128
			if program_color_map.size() > 0 and color_index < program_color_map.size():
				return program_color_map[color_index]
			else:
				return COLOR_FALLBACK
		else:
			# Fallback if out of range
			if program_color_map.size() == 0:
				return COLOR_FALLBACK
			var color_index: int = program % program_color_map.size()
			return program_color_map[color_index]
	else:
		# Non-percussion mode:
		# program 1-128 (0-127): get from program_color_map
		# program 129-256 (128-255): yellow
		if program >= 0 and program < 128:
			if program_color_map.size() == 0:
				return COLOR_FALLBACK
			var color_index: int = program % program_color_map.size()
			return program_color_map[color_index]
		elif program >= 128 and program < 256:
			return Color(1.0, 1.0, 0, 1)
		else:
			# Fallback: conventional logic
			if program_color_map.size() == 0:
				return COLOR_FALLBACK
			var color_index: int = program % program_color_map.size()
			return program_color_map[color_index]

func _draw():
	var rect_size = size
	
	if not has_meta("first_draw"):
		set_meta("first_draw", true)
	
	# Check if size is valid
	if rect_size.x <= 0 or rect_size.y <= 0:
		return
	
	# Calculate key width (equal spacing for all keys)
	var key_width = rect_size.x / Globalv.num_keyboard_key
	
	# Check for program change and update visibility flags for all notes
	var current_program = Globalv.program
	if current_program != last_program:
		last_program = current_program
		for n in active_notes:
			if is_percussion_overlay:
				n["visible"] = true
			else:
				if show_all_programs:
					n["visible"] = true
				else:
					n["visible"] = (n.get("program", -1) == current_program)

	# Draw key backgrounds (white keys, black keys) - Always draw, independent of music loading/playing
	var sharp_note_mods = [1, 3, 6, 8, 10]  # C#, D#, F#, G#, A# (corrected)
	for i in range(Globalv.num_keyboard_key):
		var key = Globalv.most_left_key_num + i
		var note_mod = key % 12
		var x = i * key_width
		if note_mod in sharp_note_mods:
			# Black keys
			draw_rect(Rect2(x, 0, key_width, rect_size.y), COLOR_BLACK_KEY)
		else:
			# White keys (dark gray)
			draw_rect(Rect2(x, 0, key_width, rect_size.y), COLOR_WHITE_KEY)
	
	# Draw grid lines for each key (vertical lines)
	for i in range(Globalv.num_keyboard_key + 1):
		var x = i * key_width
		draw_line(Vector2(x, 0), Vector2(x, rect_size.y), COLOR_GRID_VERTICAL, 1.0)
	
	# Draw horizontal grid lines every 1 second for time reference
	# Calculate 1 second in pixels: viewport_height represents pre_on_time seconds
	if pre_on_time > 0.0:
		var one_second_pixels: float = viewport_height / pre_on_time
		if one_second_pixels > 0.0 and not is_inf(one_second_pixels) and not is_nan(one_second_pixels):
			for i in range(int(rect_size.y / one_second_pixels) + 1):
				var y = i * one_second_pixels
				draw_line(Vector2(0, y), Vector2(rect_size.x, y), COLOR_GRID_HORIZONTAL, 1.0)
	
	# Skip note drawing if not started yet
	if start_time_offset < 0:
		return
	
	var current_time = Time.get_ticks_msec() / 1000.0 - start_time_offset
	
	# Draw active notes (scrolling from top to bottom)
	# Notes appear at y=0 (top of viewport) and scroll down over 5 seconds
	# On pre_note_on: bottom edge is at y=0, top edge is above viewport (negative y)
	# On pre_note_off: top edge is adjusted to y=0, bottom edge is below
	# Notes scroll down at scroll_speed pixels per second
	for note in active_notes:
		# Only draw notes that are visible (match currently selected program)
		if not note.get("visible", false):
			continue
		
		var key = note["key"]
		var key_index = key - Globalv.most_left_key_num
		
		if key_index < 0 or key_index >= Globalv.num_keyboard_key:
			continue
		
		# Calculate note position
		var note_x = key_index * key_width
		var note_width = key_width - 2.0  # Small gap between notes
		
		# Calculate note vertical position (scrolling from top to bottom)
		# On pre_note_on: bottom edge starts at y=0, so note_y = -note_height_pixels
		# Notes scroll down: note_y = -note_height_pixels + elapsed_time * scroll_speed
		var note_start_time = note["start_time"]
		var elapsed_time = current_time - note_start_time
		var note_height_pixels = note["note_height"]
		
		# Check if scroll_speed is valid
		if scroll_speed <= 0.0 or is_inf(scroll_speed) or is_nan(scroll_speed):
			continue
		
		# Calculate note position
		# On pre_note_on: bottom edge at y=0, so note_top_y = -note_height_pixels
		# After elapsed_time: note has moved down by elapsed_time * scroll_speed
		# So note_top_y = -note_height_pixels + elapsed_time * scroll_speed
		# On pre_note_off: top edge is adjusted to y=0 at that moment (elapsed_at_off)
		# After pre_note_off: top edge continues scrolling from y=0
		var note_top_y: float
		var actual_note_height: float
		if note["end_time"] < 0:
			# Note hasn't ended yet: bottom edge at y=0, use default height
			note_top_y = -note_height_pixels + elapsed_time * scroll_speed
			actual_note_height = note_height_pixels
		else:
			# Note has ended: top edge was adjusted to y=0 when pre_note_off was received
			# Calculate actual height from start_time to end_time
			var note_duration = note["end_time"] - note["start_time"]
			actual_note_height = note_duration * scroll_speed
			# When pre_note_off was received: elapsed_time = elapsed_at_off
			# At that moment, top edge was at: -note_height_pixels + elapsed_at_off * scroll_speed
			# We adjust it to y=0, so the offset is: 0 - (-note_height_pixels + elapsed_at_off * scroll_speed)
			# After that, top edge continues scrolling from y=0
			# So: note_top_y = 0 + (elapsed_time - elapsed_at_off) * scroll_speed
			var elapsed_at_off = note.get("elapsed_at_off", note_duration)
			note_top_y = (elapsed_time - elapsed_at_off) * scroll_speed
		var note_bottom_y = note_top_y + actual_note_height
		
		# Only draw if note is visible (within viewport)
		# Note is visible if it overlaps with viewport (y=0 to y=340)
		if note_bottom_y > 0 and note_top_y < rect_size.y:
			# Calculate visible portion
			var visible_top = max(note_top_y, 0.0)
			var visible_bottom = min(note_bottom_y, rect_size.y)
			var visible_height = visible_bottom - visible_top
			
			if visible_height > 0:
				# Get color for this note's program
				var note_program = note.get("program", -1)
				var note_color = _get_program_color(note_program)
				
				# Create fill color (RGB 0.7x, alpha 1.0) and border color (RGB 1.0x, alpha 1.0)
				var fill_color = Color(note_color.r * 0.7, note_color.g * 0.7, note_color.b * 0.7, 1.0)
				var border_color = Color(note_color.r, note_color.g, note_color.b, 1.0)
				
				# Draw filled rectangle (RGB 0.7x, alpha 1.0)
				draw_rect(Rect2(note_x + 1, visible_top, note_width, visible_height), fill_color, true)
				
				# Draw border rectangle (alpha 1.0, filled=false, width=1.0)
				draw_rect(Rect2(note_x + 1, visible_top, note_width, visible_height), border_color, false, 2.0)
	
	# Remove notes that have scrolled off screen (top edge below y=340)
	active_notes = active_notes.filter(func(n): 
		var elapsed_time = current_time - n["start_time"]
		var note_height_pixels = n["note_height"]
		var note_top_y: float
		if n["end_time"] < 0:
			# Note hasn't ended yet: bottom edge at y=0
			note_top_y = -note_height_pixels + elapsed_time * scroll_speed
		else:
			# Note has ended: top edge was adjusted to y=0 when pre_note_off was received
			var note_duration = n["end_time"] - n["start_time"]
			var elapsed_at_off = n.get("elapsed_at_off", note_duration)
			note_top_y = (elapsed_time - elapsed_at_off) * scroll_speed
		# Keep notes if top edge is still above y=340 (with some margin)
		return note_top_y < rect_size.y + 50.0
	)


