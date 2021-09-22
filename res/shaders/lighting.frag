#version 330 core

layout (location = 0) out vec4 frag_color;

in DATA
{
	vec4 position;
	vec3 normal;
	vec3 fragPos;
	vec2 uv;
	flat int texIndex;
	vec4 color;
} fs_in;

struct Light {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform sampler2D textures[32];
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;

void main() {
	
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(fs_in.normal);
	vec3 lightDir = normalize(lightPos - fs_in.fragPos);
	
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - fs_in.fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * fs_in.color.xyz;
	frag_color = vec4(result, 1.0);

	//frag_color = texture(textures[int(fs_in.texIndex)], fs_in.uv) * vec4(result, 1.0);

}