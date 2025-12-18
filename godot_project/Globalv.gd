extends Node

const most_left_key_num:int = 21
const num_keyboard_key:int = 100
const log_level:int = 1

var is_percussion_key_select:bool = false
var is_percussion_prog_select:bool = false
var is_percussion_note_select:bool = false
var percussion_key:int = -1
var percussion_prog:int = -1
var percussion_note:int = -1

var num_division:float = 5.0
var pled_pos_y:int = 462
var num_elements:int = 0
var num_execution:int = 0
var is_loaded:bool = true
var is_keyboard_clear:bool = false
var smf_filename:StringName = ""

var num_poly:int = 32
var program:int = 0
var base_tempo:int = 60
var is_percussion:int = 0

var total_gain:float = 1.0
var atack_slope_time:float = 20.0
var decay_half_life_time:float = 200.0
var sustain_rate:float = 0.12
var release_slope_time:float = 20.0
var base_vs_others_ratio:float = 0.4
var side1_vs_side2_ratio:float = 0.8
var base_offset_cent1:float = 0.0
var base_wave1:int = 1
var base_offset_cent2:float = 1200.0
var base_wave2:int = 3
var base_offset_cent3:float = 2400
var base_wave3:int = 3
var noise_ratio:float = 0.1
var noise_color_type:int = 1
var delay0_time:float = 5.0
var delay0_ratio:float = 0.2
var delay1_time:float = 11.0
var delay1_ratio:float = 0.1
var delay2_time:float = 17.0
var delay2_ratio:float = 0.01
var freq_noise_cent_range:float = 50.0
var freq_noise_type:int = 2
var fm_cent_range:float = 0.0
var fm_freq:float = 0.0
var fm_phase_offset:float = 1.5
var fm_sync:int = 0
var fm_wave:int = 2
var am_level:float = 0.0
var am_freq:float = 0.0
var am_phase_offset:float = 1.5
var am_sync:int = 0
var am_wave:int = 2

const note_names:Array[String] = [
	"  C", " C#", "  D", " D#", "  E", "  F",
	" F#", "  G", " G#", "  A", " A#", "  B"]

const insturments:Array[String] = [
	"Acoustic Piano         ",
	"Bright Piano           ",
	"Electric Grand Piano   ",
	"Honky-tonk Piano       ",
	"Electric Piano         ",
	"Electric Piano 2       ",
	"Harpsichord            ",
	"Clavi                  ",
	"Celesta                ",
	"Glockenspiel           ",
	"Music box              ",
	"Vibraphone             ",
	"Marimba                ",
	"Xylophone              ",
	"Tubular Bell           ",
	"Dulcimer               ",
	"Drawbar Organ          ",
	"Percussive Organ       ",
	"Rock Organ             ",
	"Church organ           ",
	"Reed organ             ",
	"Accordion              ",
	"Harmonica              ",
	"Tango Accordion        ",
	"Acoustic Guitar (nylon)",
	"Acoustic Guitar (steel)",
	"Electric Guitar (jazz) ",
	"Electric Guitar (clean)",
	"Electric Guitar (muted)",
	"Overdriven Guitar      ",
	"Distortion Guitar      ",
	"Guitar harmonics       ",
	"Acoustic Bass          ",
	"Electric Bass (finger) ",
	"Electric Bass (pick)   ",
	"Fretless Bass          ",
	"Slap Bass 1            ",
	"Slap Bass 2            ",
	"Synth Bass 1           ",
	"Synth Bass 2           ",
	"Violin                 ",
	"Viola                  ",
	"Cello                  ",
	"Double bass            ",
	"Tremolo Strings        ",
	"Pizzicato Strings      ",
	"Orchestral Harp        ",
	"Timpani                ",
	"String Ensemble 1      ",
	"String Ensemble 2      ",
	"Synth Strings 1        ",
	"Synth Strings 2        ",
	"Voice Aahs             ",
	"Voice Oohs             ",
	"Synth Voice            ",
	"Orchestra Hit          ",
	"Trumpet                ",
	"Trombone               ",
	"Tuba                   ",
	"Muted Trumpet          ",
	"French horn            ",
	"Brass Section          ",
	"Synth Brass 1          ",
	"Synth Brass 2          ",
	"Soprano Sax            ",
	"Alto Sax               ",
	"Tenor Sax              ",
	"Baritone Sax           ",
	"Oboe                   ",
	"English Horn           ",
	"Bassoon                ",
	"Clarinet               ",
	"Piccolo                ",
	"Flute                  ",
	"Recorder               ",
	"Pan Flute              ",
	"Blown Bottle           ",
	"Shakuhachi             ",
	"Whistle                ",
	"Ocarina                ",
	"Lead 1 (square)        ",
	"Lead 2 (sawtooth)      ",
	"Lead 3 (calliope)      ",
	"Lead 4 (chiff)         ",
	"Lead 5 (charang)       ",
	"Lead 6 (voice)         ",
	"Lead 7 (fifths)        ",
	"Lead 8 (bass + lead)   ",
	"Pad 1 (Fantasia)       ",
	"Pad 2 (warm)           ",
	"Pad 3 (polysynth)      ",
	"Pad 4 (choir)          ",
	"Pad 5 (bowed)          ",
	"Pad 6 (metallic)       ",
	"Pad 7 (halo)           ",
	"Pad 8 (sweep)          ",
	"FX 1 (rain)            ",
	"FX 2 (soundtrack)      ",
	"FX 3 (crystal)         ",
	"FX 4 (atmosphere)      ",
	"FX 5 (brightness)      ",
	"FX 6 (goblins)         ",
	"FX 7 (echoes)          ",
	"FX 8 (sci-fi)          ",
	"Sitar                  ",
	"Banjo                  ",
	"Shamisen               ",
	"Koto                   ",
	"Kalimba                ",
	"Bagpipe                ",
	"Fiddle                 ",
	"Shanai                 ",
	"Tinkle Bell            ",
	"Agogo                  ",
	"Steel Drums            ",
	"Woodblock              ",
	"Taiko Drum             ",
	"Melodic Tom            ",
	"Synth Drum             ",
	"Reverse Cymbal         ",
	"Guitar Fret Noise      ",
	"Breath Noise           ",
	"Seashore               ",
	"Bird Tweet             ",
	"Telephone Ring         ",
	"Helicopter             ",
	"Applause               ",
	"Gunshot                ",
	"Bass Drum 2            ",
	"Bass Drum 1            ",
	"Side Stick             ",
	"Snare Drum 1           ",
	"Hand Clap              ",
	"Snare Drum 2           ",
	"Low Tom 2              ",
	"Closed Hi-hat          ",
	"Low Tom 1              ",
	"Pedal Hi-hat           ",
	"Mid Tom 2              ",
	"Open Hi-hat            ",
	"Mid Tom 1              ",
	"High Tom 2             ",
	"Crash Cymbal 1         ",
	"High Tom 1             ",
	"Ride Cymbal 1          ",
	"Chinese Cymbal         ",
	"Ride Bell              ",
	"Tambourine             ",
	"Splash Cymbal          ",
	"Cowbell                ",
	"Crash Cymbal 2         ",
	"Vibra Slap             ",
	"Ride Cymbal 2          ",
	"High Bongo             ",
	"Low Bongo              ",
	"Mute High Conga        ",
	"Open High Conga        ",
	"Low Conga              ",
	"High Timbale           ",
	"Low Timbale            ",
	"High Agogo             ",
	"Low Agogo              ",
	"Cabasa                 ",
	"Maracas                ",
	"Short Whistle          ",
	"Long Whistle           ",
	"Short Guiro            ",
	"Long Guiro             ",
	"Claves                 ",
	"High Wood Block        ",
	"Low Wood Block         ",
	"Mute Cuica             ",
	"Open Cuica             ",
	"Mute Triangle          ",
	"Open Triangle          ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"                       ",
	"Tinkle Bell            ",
	"Agogo                  ",
	"Steel Drums            ",
	"Woodblock              ",
	"Taiko Drum             ",
	"Melodic Tom            ",
	"Synth Drum             ",
	"Reverse Cymbal         ",
	"Gu. Fret Noise         ",
	"Breath Noise           ",
	"Seashore               ",
	"Bird Tweet             ",
	"Telephone Ring         ",
	"Helicopter             ",
	"Applause               ",
	"Gunshot                "]

const percussions:Array[String] = [
	"                    ", # 0x00
	"                    ", # 0x01
	"                    ", # 0x02
	"                    ", # 0x03
	"                    ", # 0x04
	"                    ", # 0x05
	"                    ", # 0x06
	"                    ", # 0x07
	"                    ", # 0x08
	"                    ", # 0x09
	"                    ", # 0x0A
	"                    ", # 0x0B
	"                    ", # 0x0C
	"                    ", # 0x0D
	"                    ", # 0x0E
	"                    ", # 0x0F
	"                    ", # 0x10
	"                    ", # 0x11
	"                    ", # 0x12
	"                    ", # 0x13
	"                    ", # 0x14
	"                    ", # 0x15
	"                    ", # 0x16
	"                    ", # 0x17
	"                    ", # 0x18
	"                    ", # 0x19
	"                    ", # 0x1A
	"                    ", # 0x1B
	"                    ", # 0x1C
	"                    ", # 0x1D
	"                    ", # 0x1E
	"                    ", # 0x1F
	"                    ", # 0x20
	"                    ", # 0x21
	"                    ", # 0x22
	" Bass Drum 2        ", # 0x23
	" Bass Drum 1        ", # 0x24
	" Side Stick         ", # 0x25
	" Snare Drum 1       ", # 0x26
	" Hand Clap          ", # 0x27
	" Snare Drum 2       ", # 0x28
	" Low Tom 2          ", # 0x29
	" Closed Hi-hat      ", # 0x2A
	" Low Tom 1          ", # 0x2B
	" Pedal Hi-hat       ", # 0x2C
	" Mid Tom 2          ", # 0x2D
	" Open Hi-hat        ", # 0x2E
	" Mid Tom 1          ", # 0x2F
	" High Tom 2         ", # 0x30
	" Crash Cymbal 1     ", # 0x31
	" High Tom 1         ", # 0x32
	" Ride Cymbal 1      ", # 0x33
	" Chinese Cymbal     ", # 0x34
	" Ride Bell          ", # 0x35
	" Tambourine         ", # 0x36
	" Splash Cymbal      ", # 0x37
	" Cowbell            ", # 0x38
	" Crash Cymbal 2     ", # 0x39
	" Vibra Slap         ", # 0x3A
	" Ride Cymbal 2      ", # 0x3B
	" High Bongo         ", # 0x3C
	" Low Bongo          ", # 0x3D
	" Mute High Conga    ", # 0x3E
	" Open High Conga    ", # 0x3F
	" Low Conga          ", # 0x40
	" High Timbale       ", # 0x41
	" Low Timbale        ", # 0x42
	" High Agogo         ", # 0x43
	" Low Agogo          ", # 0x44
	" Cabasa             ", # 0x45
	" Maracas            ", # 0x46
	" Short Whistle      ", # 0x47
	" Long Whistle       ", # 0x48
	" Short Guiro        ", # 0x49
	" Long Guiro         ", # 0x4A
	" Claves             ", # 0x4B
	" High Wood Block    ", # 0x4C
	" Low Wood Block     ", # 0x4D
	" Mute Cuica         ", # 0x4E
	" Open Cuica         ", # 0x4F
	" Mute Triangle      ", # 0x50
	" Open Triangle      ", # 0x51
	"                    ", # 0x52
	"                    ", # 0x53
	"                    ", # 0x54
	"                    ", # 0x55
	"                    ", # 0x56
	"                    ", # 0x57
	"                    ", # 0x58
	"                    ", # 0x59
	"                    ", # 0x5A
	"                    ", # 0x5B
	"                    ", # 0x5C
	"                    ", # 0x5D
	"                    ", # 0x5E
	"                    ", # 0x5F
	"                    ", # 0x60
	"                    ", # 0x61
	"                    ", # 0x62
	"                    ", # 0x63
	"                    ", # 0x64
	"                    ", # 0x65
	"                    ", # 0x66
	"                    ", # 0x67
	"                    ", # 0x68
	"                    ", # 0x69
	"                    ", # 0x6A
	"                    ", # 0x6B
	"                    ", # 0x6C
	"                    ", # 0x6D
	"                    ", # 0x6E
	"                    ", # 0x6F
	"                    ", # 0x70
	"                    ", # 0x71
	"                    ", # 0x72
	"                    ", # 0x73
	"                    ", # 0x74
	"                    ", # 0x75
	"                    ", # 0x76
	"                    ", # 0x77
	"                    ", # 0x78
	"                    ", # 0x79
	"                    ", # 0x7A
	"                    ", # 0x7B
	"                    ", # 0x7C
	"                    ", # 0x7D
	"                    ", # 0x7E
	"                    "] # 0x7F
