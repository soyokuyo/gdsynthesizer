extends Node2D

func _init():
	var child = ColorRect.new()
	child.name = "test"
	child.size = Vector2(40,40)
	child.color = Color(1, 1, 1, 1)
	child.position = Vector2(10,20)
	child.scale = Vector2(1,1)
	child.modulate = Color(1, 1, 1, 1)
	child.layout_direction = 0 # LAYOUT_DIRECTION_INHERITED
	child.self_modulate = Color(1, 1, 1, 1)
	child.visible = false
	add_child(child)
	child.owner = self
# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass
