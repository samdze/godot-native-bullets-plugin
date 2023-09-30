@tool
extends EditorInspectorPlugin


var theme


func init(theme):
	self.theme = theme


func _can_handle(object):
	if object is BulletKit:
		return true
	return false


func _parse_property(object, type, path, hint, hint_text, usage, wide):
	if path == "bullet_class_name" and ClassDB.class_exists(object.bullet_class_name):
		var property_list = ClassDB.class_get_property_list(object.bullet_class_name)
		var bullet_properties_viewer = preload("bullet_kit_bullet_properties.tscn").instantiate()
		bullet_properties_viewer.theme = theme
		add_custom_control(bullet_properties_viewer)
		bullet_properties_viewer.generate_from(property_list)
		return true
	elif path == "collision_shape" and not object.collisions_enabled:
		return true
	elif path == "collision_layer" and not object.collisions_enabled:
		return true
	elif path == "collision_mask" and not object.collisions_enabled:
		return true
	elif path == "active_rect" and object.use_viewport_as_active_rect:
		return true
	elif path == "rotate":
		pass
	return false
