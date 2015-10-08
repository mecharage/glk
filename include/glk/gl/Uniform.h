#pragma once

namespace glk {
	namespace gl {

		struct Program;

		template <class T>
		struct Uniform {
			Uniform();
			Uniform(Program const &program, std::string const &name);

			Uniform(Uniform const &) = default;

			Uniform &operator = (Uniform const &) = default;

			T const &operator = (T const &rhs);

			Program &program();

			GLint location() const;

		private:
			Program const *_program;
			GLint _location;
		} ;

		using sampler2D		= int;
		using usampler2D	= int;
	}
}

#include <glk/gl/Uniform.tpp>