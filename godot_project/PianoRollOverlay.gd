extends Control

# Piano Roll Overlay for displaying notes
# Displays notes for the currently selected PROGRAM as scrolling rectangles
# Notes flow from top to bottom, divided equally for each key

var gd_synthesizer: Node = null
var active_notes: Array[Dictionary] = []  # Array of {key: int, start_time: float, end_time: float, program: int, channel: int, note_height: float}
var pre_on_time: float = 5.0  # preOnTime in seconds (matches GDSynthesizer setting)
var viewport_height: float = 340.0  # Piano roll viewport height in pixels
var scroll_speed: float = 0.0  # Pixels per second (calculated as viewport_height / pre_on_time)
var start_time_offset: float = -1.0  # Time offset for scrolling (-1 means not initialized yet)
var default_note_height: float = 0.0  # Default note height for 10 minutes (600 seconds)
var is_started: bool = false  # Whether playback has started (first note received)

func _ready():
	# Find GDSynthesizer node
	# Node structure: FrontCase -> PianoRoll -> TextureRect -> PianoRollOverlay
	var parent_node = get_parent().get_parent().get_parent()
	if parent_node:
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
	
	# Set size explicitly to match parent TextureRect
	size = Vector2(1024, 340)
	custom_minimum_size = Vector2(1024, 340)
	
	
	# Ensure Control can receive mouse events and draw children
	mouse_filter = Control.MOUSE_FILTER_IGNORE  # Ignore mouse events to pass them to underlying controls
	clip_contents = true  # Clip drawing to this node's rectangle
	
	# Force initial redraw
	queue_redraw()

var last_program: int = -1

func _on_pre_note_changed(state: String, note: Dictionary):
	# Only process notes for the currently selected PROGRAM
	var current_program = Globalv.program
	var note_program = note.get("instrumentNum", -1)
	
	if current_program != last_program:
		last_program = current_program
		# Clear active notes when program changes
		active_notes.clear()
		# Reset start time when program changes
		is_started = false
		start_time_offset = -1.0
	
	if note_program != current_program:
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
	
	var channel = note.get("channel", -1)
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
		var note_data = {
			"key": key,
			"start_time": note_start_time,  # Time when pre_note_on was received (relative to start_time_offset)
			"end_time": -1.0,  # Will be set when pre_note_off is received
			"program": note_program,
			"channel": channel,
			"note_height": default_note_height  # Initial height: 10 minutes worth (will be adjusted on pre_note_off)
		}
		active_notes.append(note_data)
		# pre_note_on handled
	elif state == "pre_note_off":
		# Find and close the first matching note (FIFO: first match from the beginning)
		for i in range(active_notes.size()):
			var n = active_notes[i]
			if n["key"] == key and n["channel"] == channel and n["program"] == note_program and n["end_time"] < 0:
				# Record the time when pre_note_off was received
				# Don't change note_height - it will be calculated from start_time to end_time in _draw()
				# Record the elapsed_time at this moment to fix the top edge position
				var elapsed_at_off = current_time - start_time_offset - n["start_time"]
				n["end_time"] = current_time - start_time_offset
				n["elapsed_at_off"] = elapsed_at_off  # Store elapsed time when pre_note_off was received
				var note_duration = n["end_time"] - n["start_time"]
				# pre_note_off handled
				break

func _process(delta):
	queue_redraw()

func _draw():
	# Skip drawing if not started yet
	if start_time_offset < 0:
		return
	
	var current_time = Time.get_ticks_msec() / 1000.0 - start_time_offset
	var rect_size = size
	
	if not has_meta("first_draw"):
		set_meta("first_draw", true)
	
	# Check if size is valid
	if rect_size.x <= 0 or rect_size.y <= 0:
		return
	
	# Calculate key width (equal spacing for all keys)
	var key_width = rect_size.x / Globalv.num_keyboard_key
	
	# Draw grid lines for each key (vertical lines) - Blue color
	for i in range(Globalv.num_keyboard_key + 1):
		var x = i * key_width
		# Blue color for better visibility
		draw_line(Vector2(x, 0), Vector2(x, rect_size.y), Color(0.2, 0.4, 1.0, 0.8), 1.0)
	
	# Draw horizontal grid lines every 50 pixels for time reference - Blue color
	var horizontal_line_spacing = 50.0
	for i in range(int(rect_size.y / horizontal_line_spacing) + 1):
		var y = i * horizontal_line_spacing
		draw_line(Vector2(0, y), Vector2(rect_size.x, y), Color(0.2, 0.4, 1.0, 0.5), 1.0)
	
	# Draw active notes (scrolling from top to bottom)
	# Notes appear at y=0 (top of viewport) and scroll down over 5 seconds
	# On pre_note_on: bottom edge is at y=0, top edge is above viewport (negative y)
	# On pre_note_off: top edge is adjusted to y=0, bottom edge is below
	# Notes scroll down at scroll_speed pixels per second
	for note in active_notes:
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
				# Draw note rectangle - Red color
				var note_color = Color(1.0, 0.0, 0.0, 0.9)  # Red color
				draw_rect(Rect2(note_x + 1, visible_top, note_width, visible_height), note_color)
	
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


