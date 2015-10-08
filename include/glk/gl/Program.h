#pragma once

#include <string>

#include <GL/glew.h>

#include <glk/gl/Shader.h>

namespace glk {
	namespace gl {

		template <class T> struct Uniform;

		struct Program {
			friend struct ProgramBuilder;

			Program();

			Program(Program const &) = delete;
			Program(Program &&other);

			~Program();

			Program &operator = (Program const &) = delete;
			Program &operator = (Program &&other);

			GLuint id() const;

			void use(bool force = false) const;

			static void unuse();

			template <class T>
			Uniform<T> uniform(std::string const &name) const;

		private:
			Program(GLuint id);

			GLuint _id;
			static GLuint _currentlyUsedId;
		} ;

		struct ProgramBuilder {
			ProgramBuilder();

			ProgramBuilder(ProgramBuilder const &) = delete;
			ProgramBuilder(ProgramBuilder &&) = delete;

			~ProgramBuilder();

			ProgramBuilder &operator = (ProgramBuilder const &) = delete;

			template <class Tshader>
			ProgramBuilder &attach(Tshader &&shader);

			Program link();

		private:
			GLuint _id;
		} ;

	}
}

#include <glk/gl/Program.tpp>
