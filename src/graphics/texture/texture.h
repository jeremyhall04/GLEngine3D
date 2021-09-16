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
	Texture(const char* path);
	~Texture();

	void load();
	void bind() const;
	void unbind() const;
	inline const GLuint getWidth() const { return m_Width; };
	inline const GLuint getHeight() const { return m_Height; };
	inline const GLuint getID() const { return m_TextureID; };
};

#endif // !TEXTURE_H
