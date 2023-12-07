@tool
extends VBoxContainer


const TYPE_MAPPINGS = {
	TYPE_ARRAY: "Array",
	TYPE_COLOR: "Color",
	TYPE_VECTOR2: "Vector2",
	TYPE_INT: "int",
	TYPE_FLOAT: "float",
	TYPE_NIL: "Null",
	TYPE_STRING: "String",
	TYPE_DICTIONARY: "Dictionary",
	TYPE_BOOL: "bool",
	TYPE_NODE_PATH: "NodePath",
	TYPE_VECTOR3: "Vector2",
	TYPE_TRANSFORM3D: "Transform3D",
	TYPE_TRANSFORM2D: "Transform2D",
	TYPE_AABB: "AABB",
	TYPE_BASIS: "Basis",
	TYPE_PACKED_COLOR_ARRAY: "PackedColorArray",
	TYPE_PACKED_INT32_ARRAY: "PackedInt32Array",
	TYPE_OBJECT: "Object",
	TYPE_PACKED_BYTE_ARRAY: "PackedByteArray",
	TYPE_PACKED_FLOAT32_ARRAY: "PackedFloat32Array",
	TYPE_PACKED_STRING_ARRAY: "PackedStringArray",
	TYPE_PACKED_VECTOR2_ARRAY: "PackedVector2Array",
	TYPE_PACKED_VECTOR3_ARRAY: "PackedVector3Array", 
	TYPE_RID: "RID",
	TYPE_RECT2: "Rect2",
	TYPE_QUATERNION: "Quaternion",
	TYPE_PLANE: "Plane",
}


func generate_from(properties_array):
	for c in get_children():
		remove_child(c)
		c.queue_free()
	
	for property in properties_array:
		var hbox = HBoxContainer.new()
		
		var editor_prop = EditorProperty.new()
		editor_prop.label = property.name.capitalize()
		editor_prop.size_flags_horizontal = SIZE_EXPAND_FILL
		
		# Setting a tooltip crashes the editor when the property is hovered.
		#editor_prop.tooltip_text = "Property: " + property.name
		#if property.name == "item_rid" or property.name == "cycle" or property.name == "shape_index":
			#editor_prop.tooltip_text += "\nRead only."
		
		var icon = Button.new()
		icon.flat = true
		icon.size_flags_horizontal = SIZE_EXPAND_FILL
		icon.clip_text = true
		
		editor_prop.add_child(icon)
		hbox.add_child(editor_prop)
		add_child(hbox)
		
		var theme = EditorInterface.get_editor_theme()
		var icon_name = TYPE_MAPPINGS[property.type]
		if property.type == TYPE_OBJECT:
			if property["class_name"].is_empty():
				icon_name = "Variant"
			else:
				icon_name = property["class_name"]
		icon.icon = theme.get_icon(icon_name, "EditorIcons")
		icon.text = icon_name
