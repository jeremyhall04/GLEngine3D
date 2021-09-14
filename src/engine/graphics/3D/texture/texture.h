#ifndef TEXTURE_H
#define TEXTURE_H

#include "../../../GLcommon.h"

namespace delta { namespace graphics {

	class Texture
	{
	private:
		const char* m_FileName;
		GLuint m_TID;
		GLuint m_Width, m_Height;
		
	public:
		Texture(const char* path);
		~Texture();

		void bind() const;
		void unbind() const;
		void loadPixels(const char* path, GLuint* width, GLuint* height);
		inline const GLuint getWidth() const { return m_Width; };
		inline const GLuint getHeight() const { return m_Height; };
	};

}}

#endif // !TEXTURE_H
