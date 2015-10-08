#pragma once

#include <string>

#include <GL/glew.h>

namespace glk {
	namespace gl {

		template <GLenum TglType>
		struct Shader {
			Shader(std::string const &source);

			Shader(Shader const &) = delete;
			Shader(Shader &&other);

			Shader &operator = (Shader &&other);

			~Shader();

			Shader &operator = (Shader const&) = delete;

			GLuint id() const;
			constexpr GLenum glType() const;

			static Shader fromFile(std::string const &fName);

		private:
			GLenum _type;
			GLuint _id;
		} ;

		using VertexShader = Shader<GL_VERTEX_SHADER>;
		using FragmentShader = Shader<GL_FRAGMENT_SHADER>;

	}
}

#include <glk/gl/Shader.tpp>
