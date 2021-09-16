#include "shader.h"

#include <iostream>
#include <sstream>
#include <fstream>

Shader::Shader()
{
	loadShaders();
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	m_ShaderID = glCreateProgram();
	loadShader(vertexPath, vertexType);
	loadShader(fragmentPath, fragmentType);
	glLinkProgram(m_ShaderID);
	glValidateProgram(m_ShaderID);
	int success;
	char infoLog[512];
	glGetProgramiv(m_ShaderID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_ShaderID, 512, NULL, infoLog);
		std::cout << "\nERROR::SHADER::PROGRAM::LINKING FAILED\n" << std::endl << infoLog;
	}
}

Shader::~Shader()
{
	glDeleteProgram(m_ShaderID);
}

void Shader::loadShaders()
{
	// --- Read shader code from shader files ---

	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vShaderFile.open(m_VertexPathConst);
		fShaderFile.open(m_FragmentPathConst);
		std::stringstream vShaderStream, fShaderStream;
		// read file buffer into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.close();
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "\nERROR::SHADER::FILE_NOT_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// --- Compile & link shaders ---

	GLuint vertex = 0, fragment = 0;
	int success;
	char infoLog[512];

	// --- Vertex Shader ---

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "\nERROR::SHADER::VERTEX::COMPILATION FAILED::" << infoLog << "\n";
	}

	// --- Fragment Shader ---

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "\nERROR::SHADER::FRAGMENT::COMPILATION FAILED::" << infoLog << "\n";
	}

	// --- Create Program ---

	m_ShaderID = glCreateProgram();
	glAttachShader(m_ShaderID, vertex);
	glAttachShader(m_ShaderID, fragment);
	glLinkProgram(m_ShaderID);
	glValidateProgram(m_ShaderID);
	glGetProgramiv(m_ShaderID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_ShaderID, 512, NULL, infoLog);
		std::cout << "\nERROR::SHADER::PROGRAM::LINKING FAILED::" << infoLog << "\n";
	}

	// --- Delete shdaers ---

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::loadShader(const char* shaderPath, shaderType type)
{
	// --- Read shader code from shader file ---

	std::string str_shaderCode;
	std::ifstream shaderFile;
	shaderFile.open(shaderPath);
	if (shaderFile.is_open())
	{
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		str_shaderCode = shaderStream.str();
	}
	else
		std::cout << "\nERROR::SHADERS::loadShader(const char* shaderPath)::Unable to open file.";

	const char* shaderCode = str_shaderCode.c_str();

	// --- Compile shaders ---

	GLuint shader = 0;
	int success;
	char infoLog[512];

	// Create shader based on shader type
	switch (type) {
	case vertexType:
		shader = glCreateShader(GL_VERTEX_SHADER);
		break;
	case fragmentType:
		shader = glCreateShader(GL_FRAGMENT_SHADER);
		break;
	}

	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "\nERROR::SHADER::FRAGMENT::COMPILATION FAILED::" << infoLog << "\n";
	}

	// --- Link shader ---

	glAttachShader(m_ShaderID, shader);
	glDeleteShader(shader);
}

void Shader::enable() const
{
	glUseProgram(m_ShaderID);
}

void Shader::disable() const
{
	glUseProgram(0);
}

int Shader::getUniformLocation(const char* name)
{
	return glGetUniformLocation(m_ShaderID, name);
}

void Shader::setUniformBool(const char* name, const bool value)
{
	glUniform1i(getUniformLocation(name), (int)value);
}

void Shader::setUniform1i(const char* name, const int value)
{
	glUniform1i(getUniformLocation(name), value);
}

void Shader::setUniform1f(const char* name, const float value)
{
	glUniform1f(getUniformLocation(name), value);
}

void Shader::setUniform2f(const char* name, const glm::vec2& vector)
{
	glUniform2f(getUniformLocation(name), vector.x, vector.y);
}

void Shader::setUniform3f(const char* name, const glm::vec3& vector)
{
	glUniform3f(getUniformLocation(name), vector.x, vector.y, vector.z);
}

void Shader::setUniform4f(const char* name, const glm::vec4& vector)
{
	glUniform4f(getUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
}

void Shader::setUniformMat4(const char* name, const glm::mat4& m)
{
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(m));
}