@icon("../icons/icon_timed_events.svg")
extends Node
class_name TimedEvents
# Defines and fires an amount of events per second over time based on a curve.


signal event()
signal event_with_leftover(leftover)

@export var enabled: bool = true: set = _set_enabled
@export var connect_to_children: NodePath
@export var duration = 1.0 # (float, 0.001, 300.0)
@export var events_per_second_over_time: Curve
@export var events_per_second_multiplier = 1.0 # (float, 0.0, 1024.0)
@export var time_offset: float = 0.0
@export var loop: bool = true

var time_passed = 0.0
var wait_time = 0.0
var remaining_time = 0.0


func _ready():
	if connect_to_children:
		var node = get_node(connect_to_children)
		if node:
			# Nodes that have a `on_timed_events_request` function will call it.
			node.propagate_call("on_timed_events_request", [self], true)
	_set_enabled(enabled)
	_update_events_per_second()


func _physics_process(delta):
	time_passed += delta
	_update_events_per_second()
	if time_passed >= duration and loop:
		# Keep time_passed small, improve precision over time.
		time_passed = fmod(time_passed, duration)
	
	remaining_time -= delta
	while remaining_time <= 0:
		_on_event_timeout(-remaining_time)
		remaining_time += wait_time


func reset():
	time_passed = 0


func _set_enabled(value):
	enabled = value
	set_physics_process(enabled)


func _update_events_per_second():
	var events_per_second = 0.0
	if loop:
		events_per_second = events_per_second_over_time.sample(fmod((time_passed + time_offset) / duration, 1.0)) * events_per_second_multiplier
	else:
		events_per_second = events_per_second_over_time.sample((time_passed + time_offset) / duration) * events_per_second_multiplier
	
	if events_per_second == 0:
		_set_wait_time(INF)
	else:
		_set_wait_time(1.0 / float(events_per_second))


func _set_wait_time(time):
	wait_time = time
	if remaining_time > wait_time:
		remaining_time = wait_time


func _on_event_timeout(leftover = 0.0):
	emit_signal("event")
	emit_signal("event_with_leftover", leftover)
