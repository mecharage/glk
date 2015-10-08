
#include <GL/glew.h>

#include <glk/stringLiterals.h>
#include <glk/error.h>
#include <glk/gl/UniformImpl.h>
#include <glk/gl/Program.h>

namespace glk {
	namespace gl {

		template <class T>
		Uniform<T>::Uniform()
		: _program(nullptr), _location(0u) { }

		template <class T>
		Uniform<T>::Uniform(Program const &program, std::string const &name)
		: _program(&program), _location(glGetUniformLocation(program.id(), name.c_str())) {
			CATCH_GL_ERROR();
			if (_location == -1)
				glk::warn("Uniform "s + name + " could not be found.");
		}

		template <class T>
		T const &Uniform<T>::operator = (T const &rhs) {
			_program->use();
			detail::access<T>::set(_location, rhs);
			CATCH_GL_ERROR();
			return rhs;
		}

		template <class T>
		Program &Uniform<T>::program() {
			return _program;
		}

		template <class T>
		GLint Uniform<T>::location() const {
			return _location;
		}
	}
}
