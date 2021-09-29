#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in float texID;
layout (location = 4) in vec4 color;

uniform mat4 pr_matrix;
uniform mat4 vw_matrix = mat4(1.0);
uniform mat4 ml_matrix = mat4(1.0);

out DATA
{
	vec4 position;
	vec3 normal;
	vec3 fragPos;
	vec2 uv;
	flat int texIndex;
	vec4 color;
} vs_out;

void main() {
	gl_Position = pr_matrix * vw_matrix * ml_matrix * vec4(position, 1.0);
	vs_out.position = ml_matrix * vec4(position, 1.0);
	vs_out.normal = normal;
	vs_out.fragPos = vec3(ml_matrix * vec4(position, 1.0));
	vs_out.uv = uv;
	vs_out.texIndex = int(texID);
	vs_out.color = color;

}