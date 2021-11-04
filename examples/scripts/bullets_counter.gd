extends Label


onready var format_string = text


func _process(_delta):
	if is_instance_valid(Bullets):
		text = format_string.format([str(Bullets.get_total_active_bullets())])
	else:
		text = format_string.format([0])
