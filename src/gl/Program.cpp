#include <glk/gl/Program.h>

#include <glk/stringLiterals.h>

namespace glk {
	namespace gl {

		GLuint Program::_currentlyUsedId = 0u;

		Program::Program()
		: Program(0u) { }

		Program::Program(GLuint id)
		: _id(id) { }

		Program::Program(Program &&other)
		: Program(other._id) {
			other._id = 0u;
		}

		Program::~Program() {
			TRY_GL(glDeleteProgram(_id));
		}

		Program &Program::operator = (Program &&other) {
			_id = other._id;
			other._id = 0u;
			return *this;
		}

		GLuint Program::id() const {
			return _id;
		}

		void Program::use(bool force) const {
			if (force || _currentlyUsedId != _id) {
				TRY_GL(glUseProgram(_id));
				_currentlyUsedId = _id;
			}
		}

		void Program::unuse() {
			TRY_GL(glUseProgram(0u));
			_currentlyUsedId = 0u;
		}

		ProgramBuilder::ProgramBuilder()
		: _id(glCreateProgram()) {
			if (!_id)
				glk::error("Could not create program : " + getError());
		}

		ProgramBuilder::~ProgramBuilder() {
			TRY_GL(glDeleteProgram(_id));
		}

		Program ProgramBuilder::link() {
			glLinkProgram(_id);

			GLint status;
			TRY_GL(glGetProgramiv(_id, GL_LINK_STATUS, &status));
			if (!status) {
				std::string log = programLog(_id);
				glk::error("Program linking error : "s + getError() + "\n"s + log);
				throw ProgramLinkingException(log);
			}

			GLuint programId = _id;
			_id = 0u;

			glk::log("Program linked");

			return Program(programId);
		}

	}
}
