tool
extends EditorInspectorPlugin


var theme


func init(theme):
	self.theme = theme


func can_handle(object):
	var script = object.get_script()
	if script != null and script is NativeScript:
		return script.get_base_script()["class_name"] == "BulletKit"
	return false


func parse_property(object, type, path, hint, hint_text, usage):
	if path == "bullet_properties":
		var native_script = NativeScript.new()
		native_script.library = preload("../bullets.gdnlib")
		native_script.set_class_name(object.bullet_class_name)
		native_script.reload()
		var bullet_properties_viewer = preload("bullet_kit_bullet_properties.tscn").instance()
		bullet_properties_viewer.theme = theme
		add_custom_control(bullet_properties_viewer)
		bullet_properties_viewer.generate_from(native_script.get_script_property_list())
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
