#include <glk/gl/Vbo.h>

namespace glk {
	namespace gl {

		template <class Attrib>
		Vbo<Attrib>::Vbo(GLuint capacity) {
			reserve(capacity);
		}

		template <class Attrib>
		template <class ContigIt>
		Vbo<Attrib>::Vbo(ContigIt first, ContigIt last) {
			assign(first, last);
		}

		template <class Attrib>
		template <class ContigIt>
		void Vbo<Attrib>::assign(ContigIt first, ContigIt last) {
			data(GLuint(std::distance(first, last)), std::addressof(*first));
		}

		template <class Attrib>
		void Vbo<Attrib>::reserve(GLuint capacity) {
			return data(capacity, nullptr);
		}

		template <class Attrib>
		void Vbo<Attrib>::data(GLuint size, Attrib const *data) {
			TRY_GL(glBindBuffer(GL_ARRAY_BUFFER, _name));

			if(size > _capacity) {
				TRY_GL(glBufferData(GL_ARRAY_BUFFER, size * sizeof(Attrib), data, GL_STATIC_DRAW));
				_capacity = size;
			} else
				TRY_GL(glBufferSubData(GL_ARRAY_BUFFER, 0, size * sizeof(Attrib), data));

			_size = size;
			TRY_GL(glBindBuffer(GL_ARRAY_BUFFER, 0u));
		}

		template <class Attrib>
		GLuint Vbo<Attrib>::name() const {
			return _name;
		}

		template <class Attrib>
		GLuint Vbo<Attrib>::size() const {
			return _size;
		}
	}
}