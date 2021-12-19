extends Node2D
class_name TimedRotator, "../icons/icon_timed_rotator.svg"
# Defines and sets the rotation of the node over time based on a curve.


export(bool) var enabled = true setget _set_enabled
export(float, 0.001, 300) var duration = 1.0
export(Curve) var orientation_over_time
export(float) var time_scale = 1.0
export(float) var time_offset = 0.0
export(float) var orientation_degrees_offset = 0.0
export(bool) var mirror_enabled = false
export(float, 0.0, 360.0) var mirror_reference_degrees = 0.0

var time_passed = 0.0


func _physics_process(delta):
	if enabled:
		var degrees = orientation_over_time.interpolate(fmod((time_passed + time_offset) / duration, 1.0)) + orientation_degrees_offset
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
