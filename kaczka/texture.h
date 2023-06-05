#pragma once

#include <glad/glad.h>
#include "exception.h"

namespace kaczka
{
	class Texture2D {
		GLuint id;
	public:
		void init() {
			glGenTextures(1, &id);
		}

		void bind() {
			glBindTexture(GL_TEXTURE_2D, id);
		}

		void set_size(int width, int height, void* data = nullptr) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, data);
		}

		void configure() {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			// a framebuffer must be bind before!
			/*GLenum drawBuffer = GL_COLOR_ATTACHMENT0;
			glFramebufferTexture2D(GL_FRAMEBUFFER, drawBuffer, GL_TEXTURE_2D, id, 0);
			glDrawBuffers(1, &drawBuffer);*/

			// Always check that our framebuffer is ok
			//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			//	THROW_EXCEPTION;
		}

		void set_wrap_mode(GLint mode) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
		}

		void set_as_read() {
			glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id, 0);
		}

		void dispose() {
			glDeleteTextures(1, &id);
		}
	};

	class TextureCUBE {
		GLuint id;
	public:
		void init() {
			glGenTextures(1, &id);
		}

		void bind() {
			glBindTexture(GL_TEXTURE_CUBE_MAP, id);
		}

		void set_size(int face, int width, int height, void* data = nullptr) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, data);
		}

		void configure() {
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			// a framebuffer must be bind before!
			/*GLenum drawBuffer = GL_COLOR_ATTACHMENT0;
			glFramebufferTexture2D(GL_FRAMEBUFFER, drawBuffer, GL_TEXTURE_CUBE_MAP, id, 0);
			glDrawBuffers(1, &drawBuffer);*/

			// Always check that our framebuffer is ok
			//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			//	THROW_EXCEPTION;
		}

		void set_as_read() {
			glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP, id, 0);
		}

		void dispose() {
			glDeleteTextures(1, &id);
		}
	};
}