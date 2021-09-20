#version 330 core

layout (location = 0) out vec4 frag_color;

in DATA
{
	vec4 position;
	vec2 uv;
	flat int texIndex;
	vec4 color;
} fs_in;

uniform sampler2D textures[32];

void main() {
	frag_color = texture(textures[int(fs_in.texIndex)], fs_in.uv);
}