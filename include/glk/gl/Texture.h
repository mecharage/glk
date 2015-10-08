#pragma once

#include <cstddef>
#include <string>

#include <GL/glew.h>

namespace glk {
	namespace gl {

		struct Texture {
			Texture();
			Texture(std::size_t width, std::size_t height, GLenum srcFormat, GLenum destFormat, void const *data);
			Texture(std::string const &fName);
			~Texture();
			
			Texture(Texture const &) = delete;
			Texture &operator = (Texture const &) = delete;
			
			GLuint id() const;
			
			//TODO: no rebinding
			void bind() const;
			void bind(GLenum textureUnit) const;
			
			static void unbind();
			
		private:
			GLuint _id;
		} ;
	}
}
