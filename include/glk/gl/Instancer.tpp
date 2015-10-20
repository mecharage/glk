#include <glk/gl/Instancer.h>

namespace glk {
	namespace gl {
		template <class Vertex>
		template <class InputIter, class>
		Instancer<Vertex>::Instancer(InputIter vertBeg, InputIter vertEnd)
			: _verticeCount(std::distance(vertBeg, vertEnd)) {
			TRY_GL(glGenBuffers(1, &_vertexVbo));
			TRY_GL(glBindBuffer(GL_ARRAY_BUFFER, _vertexVbo));

			TRY_GL(glBufferData(GL_ARRAY_BUFFER, _verticeCount * sizeof(Vertex), std::addressof(*vertBeg), GL_STATIC_DRAW));

			TRY_GL(glBindBuffer(GL_ARRAY_BUFFER, 0u));
		}

		template <class Vertex>
		Instancer<Vertex>::Instancer(gl::Instancer<Vertex> &&other)
			: _vertexVbo{other._vertexVbo}
			, _verticeCount{other._verticeCount} {
			other._vertexVbo = 0u;
			other._verticeCount = 0u;
		}

		template <class Vertex>
		Instancer<Vertex>::~Instancer() {
			TRY_GL(glDeleteBuffers(1, &_vertexVbo));
		}

		template <class Vertex>
		Instancer<Vertex> &Instancer<Vertex>::operator =(Instancer &&rhs) {
			TRY_GL(glDeleteBuffers(1, &_vertexVbo));
			_vertexVbo = rhs._vertexVbo;
			_verticeCount = rhs._verticeCount;
			rhs._vertexVbo = 0u;
			rhs._verticeCount = 0u;
			return *this;
		}

		template <class Vertex>
		GLuint Instancer<Vertex>::verticeCount() const {
			return _verticeCount;
		}

		template <class Vertex>
		GLuint Instancer<Vertex>::vertexVbo() const {
			return _vertexVbo;
		}

		template <class Vertex>
		template <class Attrib>
		InstanceQueue<Vertex, Attrib> Instancer<Vertex>::makeQueue() const {
			return {*this};
		}

		template <class Vertex, class Attrib>
		InstanceQueue<Vertex, Attrib>::InstanceQueue(Instancer<Vertex> const &instancer)
			: _instancer{instancer}
			, _capacity{0u}
			, _dirty{false} {
			TRY_GL(glGenVertexArrays(1, &_attrVao));
			TRY_GL(glBindVertexArray(_attrVao));

			// Running index of shader attribute locations
			GLuint locIdx = 0u;
			using attribPointers::setAttribPointers;

			TRY_GL(glBindBuffer(GL_ARRAY_BUFFER, _instancer.vertexVbo()));
			{
				GLuint prevLoc = locIdx;
				setAttribPointers(locIdx, sizeof(Vertex), 0u, attribPointers::tag<Vertex>{});
				for(GLuint i = prevLoc; i < locIdx; ++i)
					TRY_GL(glEnableVertexAttribArray(i));
			}

			TRY_GL(glGenBuffers(1, &_attrVbo));
			TRY_GL(glBindBuffer(GL_ARRAY_BUFFER, _attrVbo));
			{
				GLuint prevLoc = locIdx;
				setAttribPointers(locIdx, sizeof(Attrib), 0u, attribPointers::tag<Attrib>{});
				for(GLuint i = prevLoc; i < locIdx; ++i) {
					TRY_GL(glEnableVertexAttribArray(i));
					TRY_GL(glVertexAttribDivisor(i, 1u));
				}
			}

			TRY_GL(glBindVertexArray(0u));
		}

		template <class Vertex, class Attrib>
		InstanceQueue<Vertex, Attrib>::InstanceQueue(InstanceQueue &&other)
			: _instancer{other._instancer}
			, _attrVao{other._attrVao}
			, _attrVbo{other._attrVbo}
			, _attribs{std::move(other._attribs)}
			, _capacity{other._capacity} {
			other._attrVao = 0u;
			other._attrVbo = 0u;
			other._capacity = 0u;
		}

		template <class Vertex, class Attrib>
		InstanceQueue<Vertex, Attrib>::~InstanceQueue() {
			TRY_GL(glDeleteVertexArrays(1, &_attrVbo));
			TRY_GL(glDeleteBuffers(1, &_attrVao));
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
			TRY_GL(glBindVertexArray(_attrVao));
			TRY_GL(glBindBuffer(GL_ARRAY_BUFFER, _attrVbo));

			if(_attribs.capacity() > _capacity) {
				_capacity = _attribs.capacity();
				TRY_GL(glBufferData(GL_ARRAY_BUFFER, _capacity * sizeof(Attrib), nullptr, GL_DYNAMIC_DRAW));
			}

			TRY_GL(glBufferSubData(GL_ARRAY_BUFFER, 0u, _attribs.size() * sizeof(Attrib), _attribs.data()));

			_dirty = false;
		}

		template <class Vertex, class Attrib>
		void InstanceQueue<Vertex, Attrib>::display() {
			if(_dirty)
				upload();
			else
				TRY_GL(glBindVertexArray(_attrVao));

			TRY_GL(glBindBuffer(GL_ARRAY_BUFFER, _instancer.vertexVbo()));
			TRY_GL(glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, _instancer.verticeCount(), _attribs.size()));
		}
	}
}