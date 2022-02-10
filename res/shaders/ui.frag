#version 330 core

out vec4 FragColor;

in vec2 uv;

uniform sampler2D tex;

void main()
{
	FragColor = texture(tex, uv);
	//FragColor = vec4(0.0, 0.0, 1.0, 0.5);
}
