#version 330 core

layout (location = 0) out vec4 color;

uniform vec3 light_pos;

in DATA
{
	vec4 position;
	vec4 color;
} fs_in;

void main()
{
	float intensity = 1.0 / length(fs_in.position.xyz - light_pos);
	color = fs_in.color * intensity * 0.5;
	color = fs_in.color;
}
