tool
extends EditorInspectorPlugin


func _init():
	pass


func can_handle(object):
	return object.get_script() == preload("../bullets.gdns")


func parse_property(object, type, path, hint, hint_text, usage):
	
#	if path == "bullet_kits":
#		# Add every custom property editor to the inspector.
#		var bullet_kits_property_editor = preload("bullet_kits_property_editor.tscn").instance()
#		add_property_editor_for_multiple_properties("Bullet Kits", PoolStringArray(["bullet_kits", "pools_sizes", "z_indices"]), bullet_kits_property_editor)
#
#		return true
	if path == "bullet_kits" or path == "pools_sizes" or path == "z_indices":
		return true
#	elif path == "_add_property":
#		var adder = preload("../inspector/PropertyAdder.tscn").instance()
#		adder.object = object
#		adder.target_node = object.target_node
#		adder.connect("property_selected", self, "_on_ModifierAdder_property_selected")
#		add_custom_control(adder)
#		return true
#	elif regex.search(path) != null:
#		var result = regex.search(path)
#		var strings = result.get_string().split("/")
#		if strings[1] == "_add_modifier" and strings.size() == 2:
#			var separator = preload("../inspector/ModifierSeparator.tscn").instance()
#			add_custom_control(separator)
#			var adder = preload("../inspector/ModifierAdder.tscn").instance()
#			adder.object = object
#			adder.target_node = object.target_node
#			adder.property = strings[0]
#			add_custom_control(adder)
#		elif strings.size() >= 3 and strings[2] == "active":
#			var active_property_row = preload("../inspector/ActivePropertyRow.tscn").instance()
#			add_property_editor(path, active_property_row)
#		else:
#			return false
#		return true
#	else:
	return false


func parse_category(object, category):
	pass


func parse_end():
	pass
