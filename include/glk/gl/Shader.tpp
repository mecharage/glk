#include <glk/stringLiterals.h>

#include <GL/glew.h>

#include <glk/error.h>
#include <glk/gl/util.h>
#include <glk/gl/Shader.h>

namespace glk {
	namespace gl {

		template <GLenum TglType>
		Shader<TglType>::Shader(std::string const &source)
		: _id(glCreateShader(TglType)) {
			CATCH_GL_ERROR();

			char const *src = source.c_str();
			TRY_GL(glShaderSource(_id, 1, &src, nullptr));
			glCompileShader(_id);
			int status;
			TRY_GL(glGetShaderiv(_id, GL_COMPILE_STATUS, &status));

			if (!status) {
				std::string log = shaderLog(_id);
				glk::error("Shader compilation error : "s + getError() + "\n"s + log);
				throw ShaderCompilationException(source, log);
			}
			glk::log("Shader compiled");
		}

		template <GLenum TglType>
		Shader<TglType>::Shader(Shader &&other)
		: _id(other._id) {
			other._id = 0u;
		}

		template <GLenum TglType>
		Shader<TglType>::~Shader() {
			TRY_GL(glDeleteShader(_id));
		}

		template <GLenum TglType>
		Shader<TglType> &Shader<TglType>::operator = (Shader &&other) {
			_id = other._id;
			other._id = 0u;
			return *this;
		}

		template <GLenum TglType>
		GLuint Shader<TglType>::id() const {
			return _id;
		}

		template<GLenum TglType>
		constexpr GLenum Shader<TglType>::glType() const {
			return TglType;
		}

		template<GLenum TglType>
		Shader<TglType> Shader<TglType>::fromFile(const std::string& fName) {
			return Shader(glk::dumpFile(fName));
		}

	}
}
