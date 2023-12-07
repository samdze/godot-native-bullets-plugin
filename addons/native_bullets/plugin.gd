@tool
extends EditorPlugin


var bullets_inspector_plugin
var bullet_kit_inspector_plugin


func _enter_tree():
	bullets_inspector_plugin = preload("inspector/bullets_environment_inspector.gd").new()
	bullets_inspector_plugin.init()
	bullet_kit_inspector_plugin = preload("inspector/bullet_kit_inspector.gd").new()
	add_inspector_plugin(bullets_inspector_plugin)
	add_inspector_plugin(bullet_kit_inspector_plugin)
	
	add_autoload_singleton("Bullets", "res://addons/native_bullets/bullets.tscn")


func _exit_tree():
	remove_inspector_plugin(bullets_inspector_plugin)
	remove_inspector_plugin(bullet_kit_inspector_plugin)
	bullets_inspector_plugin = null
	bullet_kit_inspector_plugin = null
	
	remove_autoload_singleton("Bullets")
