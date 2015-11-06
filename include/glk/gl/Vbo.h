#pragma once

#include <vector>

#include <GL/glew.h>

#include <glk/gl/ObjectNames.h>

namespace glk {
	namespace gl {

		template <class Attrib>
		struct Vbo {

			Vbo() = default;
			Vbo(GLuint capacity);

			template <class ContigIt>
			Vbo(ContigIt first, ContigIt last);

			Vbo(Vbo &&other) = default;
			Vbo &operator =(Vbo &&rhs) = default;

			Vbo(Vbo const &) = delete;
			Vbo &operator =(Vbo const &) = delete;

			void reserve(GLuint capacity);
			void data(GLuint size, Attrib const *data);

			template <class ContigIt>
			void assign(ContigIt first, ContigIt last);

			GLuint name() const;

		private:
			VboName _name;
			GLuint _capacity = 0u;
		};
	}
}

#include <glk/gl/Vbo.tpp>