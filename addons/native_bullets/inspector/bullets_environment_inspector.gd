tool
extends EditorInspectorPlugin


var theme: Theme
var properties_regex: RegEx


func _init():
	properties_regex = RegEx.new()
	properties_regex.compile("bullet_type_\\d*/controls")


func init(theme: Theme):
	self.theme = theme


func can_handle(object):
	return object is BulletsEnvironment


func parse_property(object, type, path, hint, hint_text, usage):
	if path == "bullet_kits" or path == "pools_sizes" or path == "parents_hints" or path == "z_indices":
		return true
	
	var result = properties_regex.search(path)
	if result != null:
		var index = result.get_string().split("/")[0].split("_")[2].to_int()
		
		var controls = preload("bullets_environment_controls.tscn").instance()
		controls.object = object
		controls.index = index
		controls.editor_theme = theme
		add_custom_control(controls)
		return true
	
	return false
