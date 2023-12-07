# Godot Native Bullets

Efficiently spawn and move high amounts of objects like bullets for bullet hells, particles and more.

<p align="center">
  <img src="https://user-images.githubusercontent.com/19392104/138730996-4ae0c6c0-6283-44df-ba44-b1f261c9035c.gif" width="360" />
  <img src="https://user-images.githubusercontent.com/19392104/138732303-21ab282f-3f00-417f-ba47-d6e1713648ea.gif" width="360" />
</p>

This is a **GDExtension plugin**, compatible with Godot 4.2 and up.<br>
The Godot 3.x compatible version of the plugin can be found [here](https://github.com/samdze/godot-native-bullets-plugin/tree/3.x).

The versions of the pre-built binaries are as follows:

- Windows (x86-64, v2.0)
- Linux (x86-64, v2.0)
- macOS Universal (x86-64 + arm64, v2.0)

**Notice**: to make sure you have the latest binary version you can build from source.<br>
See [Compiling and extending the plugin](#compiling-and-extending-the-plugin).

## Features

1. Spawn and move high amounts of bullets.
2. Configure multiple types of bullets, each with its own behaviour.
3. Set custom properties to categories of bullets or to any single bullet.
4. Customize bullets using shaders.
5. Configure bullets with a collision shape, layer and mask of your choice.
6. Toggle collision detection on or off for each type of bullet.
7. Set the pool size and z index for every bullet type.
8. Choose the target viewport or canvas layer.
9. Extend the plugin using C++.

## Step by step

### Installation

1. Copy the addons/native_bullets folder in your addons folder.
2. Navigate to Project -> Project Settings -> Plugin and enable the Native Bullets plugin.

For best performance, toggle `physics/2d/use_bvh` off in Project Settings.

### BulletKit creation

The first thing to do is create a BulletKit resource and choose how bullets will appear and behave tweaking its properties.

1. Create a new BulletKt resource.
   In this example, we'll create a BasicBulletKit from the resource list.
   Selecting it, its properties will appear in the resource inspector.
2. To get started, fill the `texture` property with any texture you have.
3. As the `material`, you can create a new material resource using `animated_shader.gdshader` as the shader. You can find it in the utils folder.
   This shader is optional, it takes care of animating your bullets if you specify more than 1 frame in its parameters. You can customize it, create your own or use a standard material.

4. For now, turn off `collisions_enabled`, turn `use_viewport_as_active_rect` on, turn `auto_rotate` off and set `unique_modulate_component` to `None`.
   See the [Reference](#reference) section to learn more.

<p align="center">
  <img src="https://github.com/samdze/godot-native-bullets-plugin/assets/19392104/8a44c279-d63b-436c-b8cd-ec014e006166.png" />
</p>

This BasicBulletKit resource is now ready to be used!

### BulletsEnvironment node

In order to spawn bullets in the scene, a BulletsEnvironment node is needed.
This is the node that will define which kinds of bullets will be available during its lifetime.
Only one BulletsEnvironment node can be active at the same time and if you add another one to the scene tree its configuration will replace the previous one.

The BulletsEnvironment node has to be configured to choose which kinds of bullets will be able to spawn, their maximum amount and their z indices.

1. Create a new BulletsEnvironment node.
2. Choose how many types of bullets this node will be able to spawn. Increase `bullet_types_amount` to 1 for now.

3. A new section of the inspector will appear below. Unfold the `Bullet Type 0` entry.
   Here, drag & drop the BulletKit resource you created earlier to let the node know that you'll want to spawn the bullet described in it!
4. Choose the maximum amount of bullets setting the `pool_size` property and the their `z_index`.
   3000 and 1 will be ok.
   Leave the `parent_hint` property empty, it will make the bullets spawn in the nearest Viewport or CanvasLayer up in the scene tree.

<p align="center">
  <img src="https://github.com/samdze/godot-native-bullets-plugin/assets/19392104/860c6fcb-69a6-4ddf-8912-270095896037.png" />
</p>

Nice! Now the bullets are ready.

### Spawn bullets

The last step is to spawn a lot of those bullets.<br>
You can use the utilities provided by the plugin to spawn bullets in a variety of ways, but let's do it manually for now.<br>
Create a script.

```gdscript
# Assign a valid BulletKit via the inspector.
export(BulletKit) var bullet_kit


func _process(delta):
	if Input.is_key_pressed(KEY_SPACE):
		shoot()
	if Input.is_key_pressed(KEY_RIGHT):
		rotation += PI * delta
	if Input.is_key_pressed(KEY_LEFT):
		rotation -= PI * delta


func shoot():
	var bullet_velocity = Vector2(cos(global_rotation), sin(global_rotation)) * 30.0

	# Define which properties will be set to the newly spawned bullet.
	# The bullet will be spawned in the same position as this node,
	# travelling in the direction defined by its rotation.
	var properties = {
		"transform": Transform2D(global_rotation, global_position),
		"velocity": bullet_velocity
	}
	# Spawn a bullet using the selected BulletKit and setting the properties defined above.
	# Bullets is an autoload.
	Bullets.spawn_bullet(bullet_kit, properties)
```

Here you go!
Now attach this script to one of your 2D nodes, start the game, press space to shoot and right/left to change the bullets direction!

## More examples

The repository contains an `examples` folder containing a few pre-configured scenes, ready to be played in the editor.<br>

<p align="center">
  <img src="https://user-images.githubusercontent.com/19392104/140408897-00f6de24-e826-4463-a95e-15f7152ead5b.gif" width="580" />
  <img src="https://user-images.githubusercontent.com/19392104/140408824-e8008e68-5282-46b8-ab83-6dba3d96e41f.gif" width="580" />
</p>

Below are reported some of the most common things you can do.

### Bullets spawning

Spawn bullets.

```gdscript
export(float) var bullets_speed = 50.0


func shoot():
	var bullet_kit = preload("res://path/to/following_bullet_kit.tres")

	# The bullet will be spawned in the same position as this node,
	# travelling in the direction defined by its rotation.
	var bullet_velocity = Vector2(cos(global_rotation), sin(global_rotation)) * bullets_speed
	# Follow the first node inside the Player group.
	var target_node = get_tree().get_nodes_in_group("Player")[0]

	# Define which properties will be set to the newly spawned bullet.
	var properties = {
		"target_node": target_node,
		"transform": Transform2D(global_rotation, global_position),
		"velocity": bullet_velocity
	}
	# Spawn a bullet using the selected BulletKit and setting the properties defined above.
	Bullets.spawn_bullet(bullet_kit, properties)
```

### Bullets handling

You can manipulate and set properties of bullets even after you spawned them.

```gdscript
export(float) var bullets_speed = 50.0


func shoot():
	var bullet_kit = preload("res://path/to/basic_bullet_kit.tres")

	# Use `obtain_bullet` to receive an opaque BulletID object.
	var bullet_id = Bullets.obtain_bullet(bullet_kit)

	# You can then use `bullet_id' to set and get properties.
	Bullets.set_bullet_property(bullet_id, "transform", Transform2D(global_rotation, global_position))
	Bullets.set_bullet_property(bullet_id, "velocity", Vector2(cos(global_rotation), sin(global_rotation)) * bullets_speed)
```

### Collision detection

When a bullet collides with a body or an area, the best way to get the data of the colliding bullet is to connect to the `area_shape_entered` signal.

```gdscript
func _on_area_shape_entered(area_id, _area, area_shape, _local_shape):
	if not Bullets.is_bullet_existing(area_id, area_shape):
		# The colliding area is not a bullet, returning.
		return

	# Get a BulletID from the area_shape passed in by the engine.
	var bullet_id = Bullets.get_bullet_from_shape(area_id, area_shape)

	# Get bullet properties, transform, velocity, lifetime etc.
	var bullet_transform = Bullets.get_bullet_property(bullet_id, "transform")
	# If you previously set a custom Dictionary containing the `damage` key as the data property, you can retrieve it.
	var bullet_damage = Bullets.get_bullet_property(bullet_id, "data").damage

	# You can also retrieve the BulletKit that generated the bullet and get/set its properties.
	var kit_collision_shape = Bullets.get_kit_from_bullet(bullet_id).collision_shape

	# Remove the bullet, call_deferred is necessary because the PhysicsServer2D is in its flushing state during callbacks.
	Bullets.call_deferred("release_bullet", bullet_id)
```

## Reference

The plugin has a few main components, explained below:

### @ BulletKit

This is the resource used to configure each type of bullet.<br>
Every BulletKit spawns its own bullet instances that support different kinds of properties.<br>

The plugin comes with 4 base variants that can be customized via the editor:

<details>
<summary>BasicBulletKit</summary>
<br>
This is the most basic BulletKit, makes bullets move, generally at constant speed.<br>
It's configurable with:

- `texture`: controls what texture is sent to the bullet material.
- `material`: the material used to render each bullet.
- `collisions_enabled`: enables or disables collision detection, turning it off increases performance.
- `collision_layer`: the collision layer to use during collision detection. Visible only if `collisions_enabled` is on.
- `collision_mask`: the collision mask to use during collision detection. Visible only if `collisions_enabled` is on.
- `collision_shape`: the CollisionShape to use during collision detection. Visible only if `collisions_enabled` is on.
- `use_viewport_as_active_rect`: if enabled, uses the current viewport to detect whether a bullet should be deleted.
- `active_rect`: the rect outside of which the bullets get deleted. Visible only if `use_viewport_as_active_rect` if off.
- `auto_rotate`: controls whether the bullets automatically rotate based on their direction of travel.
- `unique_modulate_component`: controls which modulate component in the material will be used as a unique value for each bullet instance. This can be used to offset bullets animation frames by unique amounts inside shaders and it's needed due to Godot not supporting material instance properties in 3.x.
- `data`: custom data you can assign to the BulletKit.

Bullets spawned by a BasicBulletKit have those properties:

- `item_rid`: the bullet canvas item RID, used internally. Read-only.
- `cycle`: the number of times the bullet was recycled, used internally. Read-only.
- `shape_index`: the index of the collision shape used by this bullet, used internally. Read-only.
- `transform`: the Transform2D used to position and rotate the bullet.
- `velocity`: the Vector2 that will be used to update the bullet position.
- `lifetime`: how much time the bullet has been alive.
- `data`: custom data you can assign to any bullet.
</details>

<details>
<summary>FollowingBulletKit</summary>
<br>
This BulletKit lets you define a target node for each bullet and a turning speed used to chase it around.<br>
It's configurable with:

- `texture`: controls what texture is sent to the bullet material.
- `bullets_turning_speed`: the turning speed with which the bullets will rotate towards the target node.
- `material`: the material used to render each bullet.
- `collisions_enabled`: enables or disables collision detection, turning it off increases performance.
- `collision_layer`: the collision layer to use during collision detection. Visible only if `collisions_enabled` is on.
- `collision_mask`: the collision mask to use during collision detection. Visible only if `collisions_enabled` is on.
- `collision_shape`: the CollisionShape to use during collision detection. Visible only if `collisions_enabled` is on.
- `use_viewport_as_active_rect`: if enabled, uses the current viewport to detect whether a bullet should be deleted.
- `active_rect`: the rect outside of which the bullets get deleted. Visible only if `use_viewport_as_active_rect` if off.
- `auto_rotate`: controls whether the bullets automatically rotate based on their direction of travel.
- `unique_modulate_component`: controls which modulate component in the material will be used as a unique value for each bullet instance. This can be used to offset bullets animation frames by unique amounts inside shaders and it's needed due to Godot not supporting material instance properties in 3.x.
- `data`: custom data you can assign to the BulletKit.

Bullets spawned by a FollowingBulletKit have those properties:

- `target_node`: the Node2D the bullet will chase.
- `item_rid`: the bullet canvas item RID, used internally. Read-only.
- `cycle`: the number of times the bullet was recycled, used internally. Read-only.
- `shape_index`: the index of the collision shape used by this bullet, used internally. Read-only.
- `transform`: the Transform2D used to position and rotate the bullet.
- `velocity`: the Vector2 that will be used to update the bullet position.
- `lifetime`: how much time the bullet has been alive.
- `data`: custom data you can assign to any bullet.
</details>

<details>
<summary>DynamicBulletKit</summary>
<br>
Lets you define bullets that change their behaviour based on their lifetime.<br>
It's configurable with:

- `texture`: controls what texture is sent to the bullet material.
- `lifetime_curves_span`: the reference lifespan used by the curves to determine their current value.
- `lifetime_curves_loop`: controls whether the curves will loop around when `lifetime` goes past `lifetime_curves_span`.
- `speed_multiplier_over_lifetime`: controls the bullet speed multiplying its velocity magnitude by the value defined in this curve, based on the bullet lifetime.
- `rotation_offset_over_lifetime`: controls the bullet rotation offsetting its initial rotation by the value in radians defined in this curve, based on the bullet lifetime.
- `material`: the material used to render each bullet.
- `collisions_enabled`: enables or disables collision detection, turning it off increases performance.
- `collision_layer`: the collision layer to use during collision detection. Visible only if `collisions_enabled` is on.
- `collision_mask`: the collision mask to use during collision detection. Visible only if `collisions_enabled` is on.
- `collision_shape`: the CollisionShape to use during collision detection. Visible only if `collisions_enabled` is on.
- `use_viewport_as_active_rect`: if enabled, uses the current viewport to detect whether a bullet should be deleted.
- `active_rect`: the rect outside of which the bullets get deleted. Visible only if `use_viewport_as_active_rect` if off.
- `auto_rotate`: controls whether the bullets automatically rotate based on their direction of travel.
- `unique_modulate_component`: controls which modulate component in the material will be used as a unique value for each bullet instance. This can be used to offset bullets animation frames by unique amounts inside shaders and it's needed due to Godot not supporting material instance properties in 3.x.
- `data`: custom data you can assign to the BulletKit.

Bullets spawned by a DynamicBulletKit have those properties:

- `transform`: the Transform2D used to position and rotate the bullet.
- `starting_transform`: the baseline Transform2D, curves will apply their changes based on this transform. This is automatically set when you manually edit `transform`.
- `velocity`: the Vector2 that will be used to update the bullet position.
- `starting_speed`: the baseline speed, curves will apply their changes based on this speed value. This is automatically set when you manually edit `velocity`.
- `item_rid`: the bullet canvas item RID, used internally. Read-only.
- `cycle`: the number of times the bullet was recycled, used internally. Read-only.
- `shape_index`: the index of the collision shape used by this bullet, used internally. Read-only.
- `lifetime`: how much time the bullet has been alive.
- `data`: custom data you can assign to any bullet.
</details>

<details>
<summary>FollowingDynamicBulletKit</summary>
<br>
Lets you define bullets that change their chasing behaviour based on their lifetime, their distance to the target, or their angle to the target.<br>
It's configurable with:

- `texture`: controls what texture is sent to the bullet material.
- `lifetime_curves_span`: the reference lifespan used by the curves to determine their current value. Only used when mode `Based On Lifetime` is selected on a curve.
- `distance_curves_span`: the reference max distance used by the curves to determine their current value. Only used when mode `Based On Target Distance` is selected on a curve.
- `lifetime_curves_loop`: controls whether the curves will loop around when `lifetime` goes past `lifetime_curves_span`. Only used when mode `Based On Lifetime` is selected on a curve.
- `speed_control_mode`: specifies the mode the speed multiplier curve will determine its value:
  - `Based On Lifetime`: the curve x axis will map to the lifetime of the bullet, from 0 to `lifetime_curves_span`.
  - `Based On Target Distance`: the curve x axis will map to the distance to the target, from 0 to `distance_curves_span`.
  - `Based on Angle To Target`: the curve x axis will map to the angle between the bullet `velocity` Vector2 and the Vector2 pointing from the bullet to the target node, from 0 to PI.
- `speed_multiplier`: controls the bullet speed multiplying its velocity magnitude by the value defined in this curve, based on whatever is set in `speed_control_mode`.
- `turning_speed_control_mode`: specifies the mode the turning speed curve will determine its value:
  - `Based On Lifetime`: the curve x axis will map to the lifetime of the bullet, from 0 to `lifetime_curves_span`.
  - `Based On Target Distance`: the curve x axis will map to the distance to the target, from 0 to `distance_curves_span`.
  - `Based on Angle To Target`: the curve x axis will map to the angle between the bullet `velocity` Vector2 and the Vector2 pointing from the bullet to the target node, from 0 to PI.
- `turning_speed`: controls the bullet turning speed towards the target node, based on whathever is set in `turning_speed_control_mode`.
- `material`: the material used to render each bullet.
- `collisions_enabled`: enables or disables collision detection, turning it off increases performance.
- `collision_layer`: the collision layer to use during collision detection. Visible only if `collisions_enabled` is on.
- `collision_mask`: the collision mask to use during collision detection. Visible only if `collisions_enabled` is on.
- `collision_shape`: the CollisionShape to use during collision detection. Visible only if `collisions_enabled` is on.
- `use_viewport_as_active_rect`: if enabled, uses the current viewport to detect whether a bullet should be deleted.
- `active_rect`: the rect outside of which the bullets get deleted. Visible only if `use_viewport_as_active_rect` is off.
- `auto_rotate`: controls whether the bullets automatically rotate based on their direction of travel.
- `unique_modulate_component`: controls which modulate component in the material will be used as a unique value for each bullet instance. This can be used to offset bullets animation frames by unique amounts inside shaders and it's needed due to Godot not supporting material instance properties in 3.x.
- `data`: custom data you can assign to the BulletKit.

Bullets spawned by a FollowingDynamicBulletKit have those properties:

- `target_node`: the Node2D the bullet will chase.
- `velocity`: the Vector2 that will be used to update the bullet position.
- `starting_speed`: the baseline speed, curves will apply their changes based on this speed value. This is automatically set when you manually edit `velocity`.
- `item_rid`: the bullet canvas item RID, used internally. Read-only.
- `cycle`: the number of times the bullet was recycled, used internally. Read-only.
- `shape_index`: the index of the collision shape used by this bullet, used internally. Read-only.
- `transform`: the Transform2D used to position and rotate the bullet.
- `lifetime`: how much time the bullet has been alive.
- `data`: custom data you can assign to any bullet.
</details>

### @ BulletsEnvironment

The BulletsEnvironment node is responsible for defining which bullets will be spawned in the current scene.
It can be configured through the editor setting which kinds of bullets will be used, the pool sizes and the z indices.

The `parent_hint` property indicates which node to use as the starting point to search for the first available Viewport or CanvasLayer up in the scene tree.
The resulting node will then be used to render the bullets.

Note: any change to a BulletsEnvironment node at runtime needs the node to be reloaded to take effect.

#### Signals

```gdscript
# Emitted when the BulletsEnvironment is about to commit itself to configure which kind of bullets will be available.
# Can be used to customize the node configuration before applying it.
signal tree_entering(node : BulletsEnvironment)
```

#### Methods

```gdscript
# Forces the reloading of this BulletsEnvironment, making it the active one.
reload() -> void
```

### @ Bullets

Bullets is the autoload used to spawn bullets into the scene.
It can spawn bullets only if a BulletsEnvironment has been configured and added to the scene.

#### Methods

```gdscript
# Sets the currently active BulletsEnvironment and allocates the bullets it contains.
# If a BulletsEnvironment is already active, it will be disabled.
mount(bullets_environment : BulletsEnvironment) -> void

# Disables and deallocates the bullets contained in `bullets_environment` if it's the currently active BulletsEnvironment.
unmount(bullets_environment : BulletsEnvironment) -> void

# Returns the currently active BulletsEnvironment node.
get_bullets_environment() -> BulletsEnvironment

# Spawns a bullet using the passed BulletKit and setting the properties contained in the `properties` dictionary.
# Returns whether a bullet has been spawned successfully.
spawn_bullet(bullet_kit : BulletKit, properties : Dictionary) -> bool

# Spawns and returns an opaque ID of a bullet using the passed BulletKit.
obtain_bullet(bullet_kit : BulletKit) -> BulletID

# Attempts to delete the bullet referenced by the passed `bullet_id`. Returns whether the removal was successful.
release_bullet(bullet_id : BulletID) -> bool

# Returns whether the bullet referenced by `bullet_id` is still alive and valid.
is_bullet_valid(bullet_id : BulletID) -> bool

# Returns whether `kit` has been configured and is ready to be used in this Bullets instance.
is_kit_valid(kit : BulletKit) -> bool

# Returns the number of currently available bullets for the `kit` BulletKit.
get_available_bullets(kit : BulletKit) -> int

# Returns the number of currently active bullets for the `kit` BulletKit.
get_active_bullets(kit : BulletKit) -> int

# Returns the number of pooled bullets for the `kit` BulletKit.
get_pool_size(kit : BulletKit) -> int

# Returns the z index of the bullets generated by the `kit` BulletKit.
get_z_index(kit : BulletKit) -> int

# Returns the total number of currently available bullets.
get_total_available_bullets() -> int

# Returns the total number of currently active bullets.
get_total_active_bullets() -> int

# Returns whether `area_rid` and `area_shape` represent a valid and active bullet.
is_bullet_existing(area_rid : RID, area_shape : int) -> BulletID

# Returns the opaque ID of a bullet based on its area RID and its shape index.
get_bullet_from_shape(area_rid : RID, area_shape : int) -> BulletID

# Returns the BulletKit that defined the bullet referenced by the passed `bullet_id`.
get_kit_from_bullet(bullet_id : BulletID) -> BulletKit

# Sets the indicated property of the bullet referenced by `bullet_id`.
set_bullet_property(bullet_id : BulletID, property : String, value : Variant) -> void

# Returns the indicated property of the bullet referenced by `bullet_id`.
get_bullet_property(bullet_id : BulletID, property : String) -> Variant
```

### @ TimedRotator

A node that defines and sets the rotation of itself over time based on a curve.

#### Properties

```gdscript
# Enables or disables the rotating behaviour.
var enabled : bool

# The time span the curve acts on.
var duration : float

# The curve that defines the node rotation over time in degrees.
# 1 in the curve X axis represents `duration` seconds passed.
var orientation_over_time : Curve

# The time scale, used to speed up or slow down the node rotation.
var time_scale : float

# Time offset used to calculate the current node rotation.
var time_offset : float

# Rotation offset in degrees applied to calculate the current node rotation.
var orientation_degrees_offset : float

# Enables the mirroring of the curve resulting rotation.
var mirror_enabled : bool

# If `mirror_enabled` is on, defines the reference degrees used to mirror the rotation applied by the curve.
var mirror_reference_degrees : bool
```

### @ TimedEvents

A node that defines and fires an amount of events per second over time based on a curve.

#### Signals

```gdscript
# Emitted when an event should be fired.
signal event()

# Emitted when an event should be fired.
# `leftover` is how much time, in seconds, the event is late. It is never above the delta time of a physics step.
signal event_with_leftover(leftover : float)
```

#### Properties

```gdscript
# Enables or disables the event firing behaviour.
var enabled : bool

# If defined, this TimedEvents node will try to connect to `connect_to_children` and its children.
# The `on_timed_events_request` function of 'connect_to_children` node and its children will be called, if defined.
# The actual connection is their resposibility.
var connect_to_children : NodePath

# The time span the curve acts on.
var duration : float

# The curve that defines the fire rate of events over time.
# 1 in the curve X axis represents `duration` seconds passed.
var events_per_second_over_time : Curve

# Multiplier used to calculate the events per second.
var events_per_second_multiplier : float

# Time offset used to calculate the events fire rate over time.
var time_offset : float

# Defines whether the curve will loop starting from the beginning if the time passed exceeds `duration`.
var loop : bool
```

#### Methods

```gdscript
# Resets the time passed to 0.
reset() -> void
```

### @ BulletsSpawner

A simple bullets spawner that uses child nodes as spawning points.
It sets bullets velocity, position and rotation.

#### Properties

```gdscript
# Enables or disables shooting.
var enabled : bool

# The BulletKit to use to spawn bullets.
var bullet_kit : BulletKit

# The spawned bullets speed.
var bullets_speed : float

# The distance from the original spawn point at which the bullets will be spawned.
var bullets_spawn_distance : float
```

#### Methods

```gdscript
# Shoots bullets based on how many children the node has.
# Every child is a spawn point and can have its own position and rotation.
# `recover_seconds` indicates how many seconds the bullet has to recover ahead of time,
# e.g. when spawning a bullet in the middle of two physics steps.
shoot(recover_seconds : float) -> void

# Called by TimedEvents nodes when they wish to automatically connect to this node.
on_timed_events_request(timed_events : TimedEvents) -> void
```

## Compiling and extending the plugin
[Prerequisites](https://docs.godotengine.org/en/3.6/tutorials/scripting/gdnative/gdnative_c_example.html#prerequisites)

The plugin can be extended with new BulletKits that are more suitable to your specific use cases.<br>
To do so, you have to download the entire repository together with submodules, write some C++ code and recompile the plugin.

```
git clone --recursive https://github.com/samdze/godot-native-bullets-plugin
```

New BulletKits can be added creating a new header file inside the src/kits directory.
Here's an example.

```c++
// src/kits/custom_following_bullet_kit.h

#ifndef CUSTOM_FOLLOWING_BULLET_KIT_H
#define CUSTOM_FOLLOWING_BULLET_KIT_H

#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <cmath>

#include "../bullet_kit.h"

using namespace godot;


// Bullet definition.
// This is necessary only if your BulletKit needs custom efficiently accessible bullet properties.
class CustomFollowingBullet : public Bullet {
	// Godot requires you to add this macro to make this class work properly.
	GDCLASS(CustomFollowingBullet, Bullet)
public:
	Node2D* target_node = nullptr;

	// Custom setter and getter.
	void set_target_node(Node2D* node) {
		target_node = node;
	}

	Node2D* get_target_node() {
		return target_node;
	}

	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_target_node", "node"), &CustomFollowingBullet::set_target_node);
		ClassDB::bind_method(D_METHOD("get_target_node"), &CustomFollowingBullet::get_target_node);

		// Registering an Object reference property with GODOT_PROPERTY_HINT_NODE_TYPE and hint_string is just
		// a way to tell the editor plugin the type of the property, so that it can be viewed in the BulletKit inspector.
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "target_node", PROPERTY_HINT_NODE_TYPE, "Node2D",
			PROPERTY_USAGE_NO_INSTANCE_STATE, "Node2D"), "set_target_node", "get_target_node");
	}
};

// Bullet kit definition.
// Add your own properties, methods and exports.
class CustomFollowingBulletKit : public BulletKit {
	// Godot requires you to add this macro to make this class work properly.
	GDCLASS(CustomFollowingBulletKit, BulletKit)
public:
	// Use this macro to configure this bullet kit.
	// Pass the BulletKit type, the BulletsPool type and the Bullet type that will be used as the arguments.
	BULLET_KIT(CustomFollowingBulletKit, CustomFollowingBulletsPool, CustomFollowingBullet)

	Ref<Texture2D> texture;
	float bullets_turning_speed = 1.0f;

	Ref<Texture2D> get_texture() { return texture; }
	void set_texture(Ref<Texture2D> texture) {
		this->texture = texture;
	}
	
	float get_bullets_turning_speed() { return bullets_turning_speed; }
	void set_bullets_turning_speed(float speed) {
		bullets_turning_speed = speed;
	}

	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_texture", "texture"), &CustomFollowingBulletKit::set_texture);
		ClassDB::bind_method(D_METHOD("get_texture"), &CustomFollowingBulletKit::get_texture);
		ClassDB::bind_method(D_METHOD("set_bullets_turning_speed", "speed"), &CustomFollowingBulletKit::set_bullets_turning_speed);
		ClassDB::bind_method(D_METHOD("get_bullets_turning_speed"), &CustomFollowingBulletKit::get_bullets_turning_speed);

		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "texture", PROPERTY_HINT_RESOURCE_TYPE, "Texture2D",
			PROPERTY_USAGE_DEFAULT, "Texture2D"), "set_texture", "get_texture");
		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "bullets_turning_speed", PROPERTY_HINT_RANGE, "0.0,128.0",
			PROPERTY_USAGE_DEFAULT, ""), "set_bullets_turning_speed", "get_bullets_turning_speed");
	}
};

// Bullets pool definition.
// This is the class that will handle the logic linked to your custom BulletKit.
// It must extend AbstractBulletsPool.
class CustomFollowingBulletsPool : public AbstractBulletsPool<CustomFollowingBulletKit, CustomFollowingBullet> {

	void _init_bullet(CustomFollowingBullet* bullet) {
		// Initialize your bullet however you like.
	}

	void _enable_bullet(CustomFollowingBullet* bullet) {
		// Runs when a bullet is obtained from the pool and is being enabled.

		// Reset the bullet lifetime.
		bullet->lifetime = 0.0f;
		Rect2 texture_rect = Rect2(-kit->texture->get_size() / 2.0f, kit->texture->get_size());
		RID texture_rid = kit->texture->get_rid();

		// Configure the bullet to draw the kit texture each frame.
		RenderingServer::get_singleton()->canvas_item_add_texture_rect(bullet->item_rid,
			texture_rect,
			texture_rid);
	}

	void _disable_bullet(CustomFollowingBullet* bullet) {
		// Runs when a bullet is being removed from the scene.
	}

	bool _process_bullet(CustomFollowingBullet* bullet, float delta) {
		// Runs each frame for each bullet, here goes your update logic.

		// Check if target_node is still a valid object and has not been freed.
		if(!UtilityFunctions::is_instance_valid(bullet->target_node)) {
			bullet->set_target_node(nullptr);
		}
		if(bullet->target_node != nullptr) {
			// Find the rotation to the target node.
			Vector2 to_target = bullet->target_node->get_global_position() - bullet->transform.get_origin();
			float rotation_to_target = bullet->velocity.angle_to(to_target);
			float rotation_value = Math::min(kit->bullets_turning_speed * delta, std::abs(rotation_to_target));

			// Apply the rotation, capped to the max turning speed.
			bullet->velocity = bullet->velocity.rotated(Math::sign(rotation_to_target) * rotation_value);
		}
		// Apply velocity.
		bullet->transform.set_origin(bullet->transform.get_origin() + bullet->velocity * delta);

		if(!active_rect.has_point(bullet->transform.get_origin())) {
			// Return true if the bullet should be deleted.
			return true;
		}
		// Rotate the bullet based on its velocity if "auto_rotate" is enabled.
		if(kit->auto_rotate) {
			bullet->transform.set_rotation(bullet->velocity.angle());
		}
		// Bullet is still alive, increase its lifetime.
		bullet->lifetime += delta;
		// Return false if the bullet should not be deleted yet.
		return false;
	}
};

// Add this macro at the end of the file to automatically implement a few needed utilities.
// Pass the BulletKit type and the BulletsPool type as arguments.
BULLET_KIT_IMPLEMENTATION(CustomFollowingBulletKit, CustomFollowingBulletsPool)

#endif
```

Next, register you Godot classes inside the `register_types.cpp` file.

```c++
// src/register_types.cpp

#include "native_bullets.h"
...
...

#include "kits/custom_following_bullet_kit.h"

...
...

void initialize_native_bullets_module(ModuleInitializationLevel p_level) {
	...
	...

	// Custom Bullet Kits.
	ClassDB::register_class<CustomFollowingBullet>();
	ClassDB::register_class<CustomFollowingBulletKit>();
}
```

Compile the bindings and the plugin for your selected platform.

```
cd addons/native_bullets
scons platform=windows target=template_release
```

Finally, reload the editor and your new BulletKit should be available to use.<br>
You can also assign an icon to the BulletKit adding a line in the `native_bullets.gdextension` file.
