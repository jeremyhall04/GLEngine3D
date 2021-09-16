#version 330 core

layout (location = 0) out vec4 frag_color;

in DATA
{
	vec4 position;
	vec2 uv;
	vec4 color;
} fs_in;

uniform sampler2D textureSampler;

void main() {
	
	frag_color = texture(textureSampler, fs_in.uv);

}