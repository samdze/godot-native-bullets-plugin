extends Node2D


export(float, 0, 10.0) var rotation_speed = 5.0

onready var timed_events = $TimedEvents


func _process(delta):
	if Input.is_key_pressed(KEY_RIGHT):
		rotation += rotation_speed * delta
	if Input.is_key_pressed(KEY_LEFT):
		rotation -= rotation_speed * delta
	
	timed_events.enabled = Input.is_action_pressed("ui_accept")
