shader_type canvas_item;
render_mode blend_mix;

uniform float default_orientation : hint_range(-3.14159265, 3.14159265) = 0;
uniform int frames : hint_range(1, 128) = 1;
uniform float frame_duration : hint_range(0.001, 16.0) = 0.1;
uniform int texture_h_frames : hint_range(1, 128) = 1;
uniform int texture_v_frames : hint_range(1, 128) = 1;
uniform bool looping = true;
uniform vec4 modulate : hint_color = vec4(1.0, 1.0, 1.0, 1.0);

void vertex() {
	float h_frames = float(texture_h_frames);
	float v_frames = float(texture_v_frames);
	
	VERTEX.xy /= vec2(h_frames, v_frames);
	VERTEX = VERTEX * mat2(vec2(cos(default_orientation), -sin(default_orientation)),
		vec2(sin(default_orientation), cos(default_orientation)));
	float total_frames = float(frames);
	// Using the red component of modulate to offset the animation frame.
	float frame = floor(MODULATE.r / frame_duration + TIME / frame_duration);
	
	if (!looping) {
		frame = clamp(frame, 0.0, total_frames - 1.0);
	} else {
		frame = mod(frame, total_frames);
	}
	
	UV /= vec2(h_frames, v_frames);
	UV += vec2(mod(frame, h_frames) / h_frames, floor(frame / h_frames) / v_frames);
}

void fragment() {
	vec4 color = texture(TEXTURE, UV);
	
	COLOR = vec4(color.rgb * modulate.rgb, color.a * modulate.a * MODULATE.a);
}