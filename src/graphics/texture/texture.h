#ifndef TEXTURE_H
#define TEXTURE_H

#include "../../GLcommon.h"
#include "../../../res/stb_image/stb_image.h"

class Texture
{
private:
	const char* m_FileName;
	GLuint m_TextureID;
	GLsizei m_Width, m_Height;
	int m_Channels;
		
public:
	Texture();
	Texture(const char* path);
	// type = 0 : textureAtlas, 1 = skyBox
	Texture(const char* path, int type);
	Texture(const char* fileNameNoExtension, bool isSkybox);
	~Texture();

	void load();
	void loadSkybox();
	void loadAtlas();
	void bind() const;
	void bind(GLuint slot) const;
	void unbind() const;
	inline const GLuint getWidth() const { return m_Width; };
	inline const GLuint getHeight() const { return m_Height; };
	inline const GLuint getID() const { return m_TextureID; };
private:
	void flipTexture(unsigned char* data);
};

GLuint createTextureArray();

#endif // !TEXTURE_H
