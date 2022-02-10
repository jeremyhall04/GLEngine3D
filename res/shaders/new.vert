#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 UV;
layout (location = 2) in float texID;

uniform mat4 pr_matrix;
uniform mat4 vw_matrix = mat4(1.0);
uniform mat4 ml_matrix = mat4(1.0);

out vec4 out_position;
out vec2 out_UV;
flat out int texIndex;

void main() {
	gl_Position = pr_matrix * vw_matrix * ml_matrix * vec4(position, 1.0);
	out_position = ml_matrix * vec4(position, 1.0);
	out_UV = UV;
	//texIndex = int(UV.z);
	texIndex = int(texID);
}