#include <glk/gl/Texture.h>

#include <string>

#include <GL/glew.h>

#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>

#include <glk/gl/util.h>
#include <glk/SurfacePtr.h>
#include <glk/error.h>

namespace glk {
	namespace gl {

		Texture::Texture() {
			TRY_GL(glGenTextures(1, &_id));
			TRY_GL(glBindTexture(GL_TEXTURE_2D, _id));
			TRY_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
			TRY_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
			TRY_GL(glBindTexture(GL_TEXTURE_2D, 0));
		}

		Texture::Texture(std::size_t width, std::size_t height, GLenum srcFormat, GLenum destFormat, void const *data) {
			TRY_GL(glGenTextures(1, &_id));
			TRY_GL(glBindTexture(GL_TEXTURE_2D, _id));
			TRY_GL(glTexImage2D(GL_TEXTURE_2D, 0, destFormat, width, height, 0, srcFormat, GL_UNSIGNED_BYTE, data));
			TRY_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
			TRY_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
			TRY_GL(glBindTexture(GL_TEXTURE_2D, 0));
		}

		Texture::Texture(std::string const &fName) {
			auto surf = SurfacePtr {IMG_Load(fName.c_str())};
			if (!surf) {
				glk::error("Could not load texture file "s + fName);
				return;
			}

			TRY_GL(glGenTextures(1, &_id));
			TRY_GL(glBindTexture(GL_TEXTURE_2D, _id));
			TRY_GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surf->w, surf->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surf->pixels));
			TRY_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
			TRY_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
			TRY_GL(glBindTexture(GL_TEXTURE_2D, 0));
		}

		Texture::~Texture() {
			TRY_GL(glDeleteTextures(1, &_id));
		}

		GLuint Texture::id() const {
			return _id;
		}

		void Texture::bind() const {
			TRY_GL(glBindTexture(GL_TEXTURE_2D, _id));
		}

		void Texture::bind(GLenum textureUnit) const {
			TRY_GL(glActiveTexture(textureUnit));
			bind();
		}

		void Texture::unbind() {
			TRY_GL(glBindTexture(GL_TEXTURE_2D, 0u));
		}

	}
}
