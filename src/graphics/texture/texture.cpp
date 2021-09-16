#include "texture.h"

#include <iostream>
#include <fstream>

Texture::Texture(const char* fileName)
	: m_FileName(fileName)
{
	load();
}

Texture::~Texture()
{
}

void Texture::load()
{
	unsigned char* data;
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(m_FileName, &m_Width, &m_Height, &m_Channels, 0);
	if (data)
	{
		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_BGR, GL_UNSIGNED_BYTE, (const void*)data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
		printf("\nERROR::TEXTURE::LOAD::m_Data is nullptr");
	stbi_image_free(data);
	//delete data;
}

void Texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
