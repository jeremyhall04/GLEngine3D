#include "texture.h"
#include <string>
#include <iostream>
#include <fstream>

Texture::Texture()
	: m_FileName("res/images/default.png")
{
	load();
}


Texture::Texture(const char* fileName)
	: m_FileName(fileName)
{
	load();
}

Texture::Texture(const char* path, int type)
{
	m_FileName = path;
	switch(type)
	{
		case 0:	// texture Atlas
			loadAtlas();
			break;
		case 1:	// skybox
			loadSkybox();
			break;
	}
}

Texture::Texture(const char* fileNameNoExtension, bool isSkybox)
{
	m_FileName = fileNameNoExtension;
	loadSkybox();
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_TextureID);
}

void Texture::load()
{
	unsigned char* data;
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(m_FileName, &m_Width, &m_Height, &m_Channels, STBI_rgb_alpha);
	if (data)
	{
		glGenTextures(1, &m_TextureID);
		glActiveTexture(GL_TEXTURE0 + m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const void*)data);

		glGenerateMipmap(GL_TEXTURE_2D);		
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
		printf("\nERROR::TEXTURE::LOAD::File could not be read: %s", m_FileName);
	stbi_image_free(data);
	stbi_set_flip_vertically_on_load(false);
}

void Texture::loadSkybox()
{
	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureID);

	std::string names[6] = {
		"res/images/skybox/skybox0.png",
		"res/images/skybox/skybox1.png",
		"res/images/skybox/skybox2.png",
		"res/images/skybox/skybox3.png",
		"res/images/skybox/skybox4.png",
		"res/images/skybox/skybox5.png",
	};

	for (int i = 0; i < 6; i++)
	{
		unsigned char* data = stbi_load(names[i].c_str(), &m_Width, &m_Height, &m_Channels, 0/*STBI_rgb_alpha*/);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, (const void*)data);
		}
		else
			std::cout << "\nERROR::TEXTURE::LOAD::Skybox could not be read: " << names[i];
		stbi_image_free(data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Texture::loadAtlas()
{
}

void Texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

void Texture::bind(GLuint slot) const
{

}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::flipTexture(unsigned char* data)
{
	unsigned char* oData = data;
}

//GLuint createTextureArray()
//{
//	//GLuint tex;
//	//glGenTextures(1, &tex);
//	//glActiveTexture(GL_TEXTURE0);
//	//glBindTexture(GL_TEXTURE_2D_ARRAY, tex);
//
//	//glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGB, _default.getWidth(), _default.getHeight(), 2, 0, GL_RGB, GL_UNSIGNED_INT, NULL);
//	//unsigned char* data;
//	//stbi_set_flip_vertically_on_load(true);
//	//data = stbi_load(m_FileName, &m_Width, &m_Height, &m_Channels, 0);
//	//if (data)
//	//{
//	//}
//	//return GLuint();
//}
