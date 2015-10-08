#include <glk/gl/Program.h>

#include <glk/gl/Uniform.h>
#include <glk/error.h>
#include <glk/gl/util.h>

namespace glk {
	namespace gl {

		template <class T>
		Uniform<T> Program::uniform(std::string const &name) const {
			return Uniform<T>(*this, name);
		}

		template <class Tshader>
		ProgramBuilder &ProgramBuilder::attach(Tshader &&shader) {
			TRY_GL(glAttachShader(_id, shader.id()));
			return *this;
		}

	}
}
