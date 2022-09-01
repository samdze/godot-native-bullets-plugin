tool
extends Node
class_name BulletsEnvironment, "icons/icon_bullets_environment.svg"


signal tree_entering(node)

export(bool) var current = true setget _set_current
export(Array, Resource) var bullet_kits: Array
export(Array, int) var pools_sizes: Array
export(Array, NodePath) var parents_hints: Array
export(Array, int) var z_indices: Array

var properties_regex : RegEx


func _init():
	properties_regex = RegEx.new()
	properties_regex.compile("bullet_type_\\d*/\\w*")


func _enter_tree():
	if Engine.editor_hint:
		return
	emit_signal("tree_entering", self)


func _ready():
	if Engine.editor_hint:
		return
#	var i = 0
#	for parent_path in parents:
#		if parent_path != null:
#			parents[i] = get_node(parent_path)
#		else:
#			parents[i] = null
#		i += 1
	if current and is_instance_valid(Bullets):
		Bullets.mount(self)


func _exit_tree():
	if Engine.editor_hint:
		return
	if is_instance_valid(Bullets):
		Bullets.unmount(self)


func reload():
	_set_current(false)
	_set_current(true)


func _set_current(value):
	if Engine.editor_hint:
		current = value
		return
	if current != value:
		current = value
		if Bullets != null and is_instance_valid(Bullets):
			if current:
				Bullets.mount(self)
			else:
				Bullets.unmount(self)


func _get(property: String):
	if property == "bullet_types_amount":
		return bullet_kits.size()
	
	var result = properties_regex.search(property)
	if result != null:
		var strings = result.get_string().split("/")
		var prop_index = strings[0].split("_")[2].to_int()
		if bullet_kits.size() > prop_index and prop_index >= 0:
			if strings[1] == "bullet_kit":
				return bullet_kits[prop_index]
			elif strings[1] == "pool_size":
				return pools_sizes[prop_index]
			elif strings[1] == "parent_hint":
				return parents_hints[prop_index]
			elif strings[1] == "z_index":
				return z_indices[prop_index]
	return null


func _set(property: String, value):
	if property == "bullet_types_amount":
		bullet_kits.resize(value)
		pools_sizes.resize(value)
		parents_hints.resize(value)
		z_indices.resize(value)
		property_list_changed_notify()
		return true
	
	var result = properties_regex.search(property)
	if result != null:
		var strings = result.get_string().split("/")
		var prop_index = strings[0].split("_")[2].to_int()
		if bullet_kits.size() > prop_index and prop_index >= 0:
			if strings[1] == "bullet_kit":
				bullet_kits[prop_index] = value
				return true
			elif strings[1] == "pool_size":
				pools_sizes[prop_index] = value
				return true
			elif strings[1] == "parent_hint":
				parents_hints[prop_index] = value
				return true
			elif strings[1] == "z_index":
				z_indices[prop_index] = value
				return true
	return false


func _get_property_list():
	var properties = []
	
	properties.append({
		"name": "bullet_types_amount",
		"type": TYPE_INT,
		"usage": PROPERTY_USAGE_DEFAULT,
		"hint": PROPERTY_HINT_RANGE,
		"hint_string": "0,64"
	})
	
	for i in range(bullet_kits.size()):
		var format_array = [i]
		
		properties.append({
			"name": "bullet_type_{0}/bullet_kit".format(format_array),
			"type": TYPE_OBJECT,
			"usage": PROPERTY_USAGE_DEFAULT,
			"hint": PROPERTY_HINT_RESOURCE_TYPE,
			"hint_string": "Resource"
		})
		properties.append({
			"name": "bullet_type_{0}/pool_size".format(format_array),
			"type": TYPE_INT,
			"usage": PROPERTY_USAGE_DEFAULT,
			"hint": PROPERTY_HINT_RANGE,
			"hint_string": "1,65536"
		})
		properties.append({
			"name": "bullet_type_{0}/parent_hint".format(format_array),
			"type": TYPE_NODE_PATH,
			"usage": PROPERTY_USAGE_DEFAULT,
			"hint": PROPERTY_HINT_NONE
		})
		properties.append({
			"name": "bullet_type_{0}/z_index".format(format_array),
			"type": TYPE_INT,
			"usage": PROPERTY_USAGE_DEFAULT,
			"hint": PROPERTY_HINT_RANGE,
			"hint_string": "-4096,4096"
		})
		properties.append({
			"name": "bullet_type_{0}/controls".format(format_array),
			"type": TYPE_NIL,
			"usage": PROPERTY_USAGE_EDITOR,
			"hint": PROPERTY_HINT_NONE
		})
	return properties
