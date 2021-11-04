tool
extends HBoxContainer


var object: BulletsEnvironment
var index: int
var editor_theme: Theme
var icon_collisions: TextureRect
var icon_viewport: TextureRect
var icon_rotating: TextureRect
var icon_data: TextureRect


func _ready():
	icon_collisions = $Icons/CollisionShape
	icon_viewport = $Icons/ViewportAsActive
	icon_rotating = $Icons/Rotating
	icon_data = $Icons/Data
	
	icon_collisions.texture = editor_theme.get_icon("Area2D", "EditorIcons")
	icon_viewport.texture = editor_theme.get_icon("ViewportContainer", "EditorIcons")
	icon_rotating.texture = editor_theme.get_icon("ToolRotate", "EditorIcons")
	icon_data.texture = editor_theme.get_icon("FileList", "EditorIcons")
	
	_draw()
	
	$MoveUpButton.icon = editor_theme.get_icon("MoveUp", "EditorIcons")
	if index == 0:
		$MoveUpButton.disabled = true
	$MoveDownButton.icon = editor_theme.get_icon("MoveDown", "EditorIcons")
	if index == object.bullet_kits.size() - 1:
		$MoveDownButton.disabled = true
	$DeleteButton.icon = editor_theme.get_icon("Remove", "EditorIcons")


func _draw():
	icon_collisions.visible = not (object.bullet_kits[index] == null or not object.bullet_kits[index].collisions_enabled)
	icon_viewport.visible = not (object.bullet_kits[index] == null or not object.bullet_kits[index].use_viewport_as_active_rect)
	icon_rotating.visible = not (object.bullet_kits[index] == null or not object.bullet_kits[index].rotate)
	icon_data.visible = not (object.bullet_kits[index] == null or object.bullet_kits[index].data.empty())


func _on_move_up_pressed():
	var bullet_kit = object.bullet_kits[index]
	var pool_size = object.pools_sizes[index]
	var z_index = object.z_indices[index]
	
	object.bullet_kits.remove(index)
	object.pools_sizes.remove(index)
	object.z_indices.remove(index)
	object.bullet_kits.insert(index -1, bullet_kit)
	object.pools_sizes.insert(index - 1, pool_size)
	object.z_indices.insert(index - 1, z_index)
	
	object.property_list_changed_notify()


func _on_move_down_pressed():
	var bullet_kit = object.bullet_kits[index]
	var pool_size = object.pools_sizes[index]
	var z_index = object.z_indices[index]
	
	object.bullet_kits.remove(index)
	object.pools_sizes.remove(index)
	object.z_indices.remove(index)
	object.bullet_kits.insert(index + 1, bullet_kit)
	object.pools_sizes.insert(index + 1, pool_size)
	object.z_indices.insert(index + 1, z_index)
	
	object.property_list_changed_notify()


func _on_delete_pressed():
	object.bullet_kits.remove(index)
	object.pools_sizes.remove(index)
	object.z_indices.remove(index)
	
	object.property_list_changed_notify()
