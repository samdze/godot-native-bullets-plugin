tool
extends VBoxContainer


const TYPE_MAPPINGS = {
	TYPE_ARRAY: "Array",
	TYPE_COLOR: "Color",
	TYPE_VECTOR2: "Vector2",
	TYPE_INT: "int",
	TYPE_REAL: "float",
	TYPE_NIL: "Null",
	TYPE_STRING: "String",
	TYPE_DICTIONARY: "Dictionary",
	TYPE_BOOL: "bool",
	TYPE_NODE_PATH: "NodePath",
	TYPE_VECTOR3: "Vector2",
	TYPE_TRANSFORM: "Transform",
	TYPE_TRANSFORM2D: "Transform2D",
	TYPE_AABB: "AABB",
	TYPE_BASIS: "Basis",
	TYPE_COLOR_ARRAY: "PoolColorArray",
	TYPE_INT_ARRAY: "PoolIntArray",
	TYPE_OBJECT: "Object",
	TYPE_RAW_ARRAY: "PoolByteArray",
	TYPE_REAL_ARRAY: "PoolRealArray",
	TYPE_STRING_ARRAY: "PoolStringArray",
	TYPE_VECTOR2_ARRAY: "PoolVector2Array",
	TYPE_VECTOR3_ARRAY: "PoolVector3Array", 
	TYPE_RID: "RID",
	TYPE_RECT2: "Rect2",
	TYPE_QUAT: "Quat",
	TYPE_PLANE: "Plane",
}

onready var header = $Header
onready var background = $Header/Background/ColorRect
onready var header_label = $Header/Foreground/Label


func _ready():
	background.color = get_color("prop_category", "Editor")
	header_label.add_font_override("font", get_font("font", "Tree"))
	header_label.add_color_override("font_color", get_color("font_color", "Tree"))


func generate_from(properties_array):
	header = $Header
	
	for c in get_children():
		if c != header:
			remove_child(c)
			c.queue_free()
	
	for property in properties_array:
		var hbox = HBoxContainer.new()
		
		var label = Label.new()
		label.clip_text = true
		label.text = property.name.capitalize()
		label.size_flags_horizontal = SIZE_EXPAND_FILL
		label.hint_tooltip = "Property: " + property.name
		if property.name == "item_rid" or property.name == "cycle" or property.name == "shape_index":
			label.hint_tooltip += "\nRead only."
		label.mouse_filter = Control.MOUSE_FILTER_STOP
		
		var icon = ToolButton.new()
		icon.size_flags_horizontal = SIZE_EXPAND_FILL
		icon.clip_text = true
		
		hbox.add_child(label)
		hbox.add_child(icon)
		
		add_child(hbox)
		
		label.add_color_override("font_color", get_color("property_color", "Editor"))
		
		var icon_name = TYPE_MAPPINGS[property.type]
		if property.type == TYPE_OBJECT:
			if property["class_name"].empty():
				icon_name = "Variant"
			else:
				icon_name = property["class_name"]
		icon.icon = icon.get_icon(icon_name, "EditorIcons")
		icon.text = icon_name
