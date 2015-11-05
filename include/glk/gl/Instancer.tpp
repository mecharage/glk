#include <glk/gl/Instancer.h>

namespace glk {
	namespace gl {
		template <class T>
		GLuint setAttribPointers(GLuint locIdx) {
			setAttribPointers(locIdx, sizeof(T), 0u, attribTag<T>{});
			return locIdx;
		}

		template <class Vertex>
		template <class InputIter, class>
		Instancer<Vertex>::Instancer(InputIter vertBeg, InputIter vertEnd)
			: _verticeCount(std::distance(vertBeg, vertEnd)) {
			TRY_GL(glBindBuffer(GL_ARRAY_BUFFER, _vertexVbo.name()));

			TRY_GL(glBufferData(GL_ARRAY_BUFFER, _verticeCount * sizeof(Vertex), std::addressof(*vertBeg), GL_STATIC_DRAW));

			TRY_GL(glBindBuffer(GL_ARRAY_BUFFER, 0u));
		}

		template <class Vertex>
		GLuint Instancer<Vertex>::verticeCount() const {
			return _verticeCount;
		}

		template <class Vertex>
		GLuint Instancer<Vertex>::vertexVboName() const {
			return _vertexVbo.name();
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
			TRY_GL(glBindVertexArray(_attrVao.name()));

			// Running index of shader attribute locations
			GLuint locIdx = 0u;

			TRY_GL(glBindBuffer(GL_ARRAY_BUFFER, _instancer.vertexVboName()));
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
		InstanceQueue<Vertex, Attrib>::InstanceQueue(InstanceQueue &&other)
			: _instancer{other._instancer}
			, _attrVao{std::move(other._attrVao)}
			, _attrVbo{std::move(other._attrVbo)}
			, _attribs{std::move(other._attribs)}
			, _capacity{other._capacity} {
			other._capacity = 0u;
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
			TRY_GL(glBindVertexArray(_attrVao.name()));
			TRY_GL(glBindBuffer(GL_ARRAY_BUFFER, _attrVbo.name()));

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
				TRY_GL(glBindVertexArray(_attrVao.name()));

			TRY_GL(glBindBuffer(GL_ARRAY_BUFFER, _instancer.vertexVboName()));
			TRY_GL(glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, _instancer.verticeCount(), _attribs.size()));
		}

		template <class Vertex, class Attrib>
		void InstanceQueue<Vertex, Attrib>::displayAndClear() {
			display();
			clear();
		}
	}
}