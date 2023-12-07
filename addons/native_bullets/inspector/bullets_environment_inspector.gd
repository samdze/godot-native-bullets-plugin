@tool
extends EditorInspectorPlugin


var properties_regex: RegEx


func _init():
	properties_regex = RegEx.new()
	properties_regex.compile("bullet_type_\\d*/controls")


func _can_handle(object):
	return object is BulletsEnvironment


func _parse_property(object, type, path, hint, hint_text, usage, wide):
	if path == "bullet_kits" or path == "pools_sizes" or path == "parents_hints" or path == "z_indices":
		return true
	
	var result = properties_regex.search(path)
	if result != null:
		var index = result.get_string().split("/")[0].split("_")[2].to_int()
		
		var controls = preload("bullets_environment_controls.tscn").instantiate()
		controls.object = object
		controls.index = index
		add_custom_control(controls)
		return true
	
	return false
