#version 330 core

//layout (location = 0) out vec4 frag_color;
out vec4 frag_color;

in vec4 out_position;
in vec2 out_UV;
flat in int texIndex;

uniform sampler2D tex;

uniform sampler2D textures[32];

void main() {
	//frag_color = texture(tex, out_UV.xy);
	frag_color = texture(textures[texIndex], out_UV.xy);
}