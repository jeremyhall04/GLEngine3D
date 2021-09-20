#include "texture.h"

#include <iostream>
#include <fstream>

Texture::Texture()
{
}


Texture::Texture(const char* fileName)
	: m_FileName(fileName)
{
	load();
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_TextureID);
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

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, (const void*)data);
		//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, (const void*)data);
		glGenerateMipmap(GL_TEXTURE_2D);
		
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
		printf("\nERROR::TEXTURE::LOAD::m_Data is nullptr");
	stbi_image_free(data);
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
