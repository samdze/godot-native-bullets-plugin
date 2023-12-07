@icon("../icons/icon_bullet_properties.svg")
extends Node2D
class_name BulletsSpawner
# Simple bullets spawner that uses child nodes as spawning points.
# It sets bullets velocity, position and rotation.


@export var enabled: bool = true
@export var bullet_kit: Resource
@export var bullets_speed = 100.0 # (float, 0.0, 65535.0)
@export var bullets_spawn_distance = 0.0 # (float, -512.0, 512.0)

var children: Array[Node]


func _ready():
	children = get_children()


# Called by TimedEvents nodes when they wish to automatically connect to other nodes.
func on_timed_events_request(timed_events : TimedEvents):
	timed_events.connect("event_with_leftover", Callable(self, "shoot"))


func shoot(recover_seconds = 0.0):
	if not enabled:
		return
	
	for spawner in children:
		var bullet_rotation = spawner.global_rotation
		var bullet_velocity = Vector2(cos(bullet_rotation), sin(bullet_rotation)) * bullets_speed
		
		var properties = {
			"transform": Transform2D(bullet_rotation, spawner.global_position \
				+ bullet_velocity * recover_seconds \
				+ bullet_velocity.normalized() * bullets_spawn_distance),
			"velocity": bullet_velocity
		}
		# Use this assigned BulletKit to spawn a bullet.
		Bullets.spawn_bullet(bullet_kit, properties)
