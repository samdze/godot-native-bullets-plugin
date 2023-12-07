@icon("../icons/icon_timed_rotator.svg")
extends Node2D
class_name TimedRotator
# Defines and sets the rotation of the node over time based on a curve.


@export var enabled: bool = true: set = _set_enabled
@export var duration = 1.0 # (float, 0.001, 300)
@export var orientation_over_time: Curve
@export var time_scale: float = 1.0
@export var time_offset: float = 0.0
@export var orientation_degrees_offset: float = 0.0
@export var mirror_enabled: bool = false
@export var mirror_reference_degrees = 0.0 # (float, 0.0, 360.0)

var time_passed = 0.0


func _physics_process(delta):
	if enabled:
		var degrees = orientation_over_time.sample(fmod((time_passed + time_offset) / duration, 1.0)) + orientation_degrees_offset
		if mirror_enabled:
			degrees = mirror_reference_degrees + (mirror_reference_degrees - degrees)
		rotation_degrees = degrees
		
		if time_passed >= duration:
			# Keep time_passed small, improve precision over time.
			time_passed = fmod(time_passed, duration)
		
		time_passed += delta * time_scale


func _set_enabled(value):
	enabled = value
	set_physics_process(enabled)
