#include <glk/gl/Instancer.h>

namespace glk {
	namespace gl {
		template <class T>
		GLuint setAttribPointers(GLuint locIdx) {
			setAttribPointers(locIdx, sizeof(T), 0u, attribTag<T>{});
			return locIdx;
		}

		template <class Vertex, class Attrib>
		InstanceQueue<Vertex, Attrib>::InstanceQueue(Vbo<Vertex> const &vertices)
			: _vertVbo{vertices}
			, _capacity{0u}
			, _dirty{false} {
			TRY_GL(glBindVertexArray(_attrVao));

			// Running index of shader attribute locations
			GLuint locIdx = 0u;

			TRY_GL(glBindBuffer(GL_ARRAY_BUFFER, _vertVbo.name()));
			{
				GLuint prevLoc = locIdx;
				locIdx = setAttribPointers<Vertex>(locIdx);
				for(GLuint i = prevLoc; i < locIdx; ++i)
					TRY_GL(glEnableVertexAttribArray(i));
			}

			TRY_GL(glBindBuffer(GL_ARRAY_BUFFER, _attrVbo.name()));
			{
				GLuint prevLoc = locIdx;
				locIdx = setAttribPointers<Attrib>(locIdx);
				for(GLuint i = prevLoc; i < locIdx; ++i) {
					TRY_GL(glEnableVertexAttribArray(i));
					TRY_GL(glVertexAttribDivisor(i, 1u));
				}
			}

			TRY_GL(glBindVertexArray(0u));
		}

		template <class Vertex, class Attrib>
		void InstanceQueue<Vertex, Attrib>::enqueue(Attrib const &attribs) {
			_attribs.push_back(attribs);
			_dirty = true;
		}

		template <class Vertex, class Attrib>
		void InstanceQueue<Vertex, Attrib>::clear() {
			_attribs.clear();
			_dirty = false;
		}

		template <class Vertex, class Attrib>
		void InstanceQueue<Vertex, Attrib>::upload() {
			_attrVbo.assign(begin(_attribs), end(_attribs));
			_dirty = false;
		}

		template <class Vertex, class Attrib>
		void InstanceQueue<Vertex, Attrib>::display() {
			if(_dirty)
				upload();
			else
				TRY_GL(glBindVertexArray(_attrVao));

			TRY_GL(glBindBuffer(GL_ARRAY_BUFFER, _vertVbo.name()));
			TRY_GL(glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, _vertVbo.size(), _attribs.size()));
		}

		template <class Vertex, class Attrib>
		void InstanceQueue<Vertex, Attrib>::displayAndClear() {
			display();
			clear();
		}
	}
}