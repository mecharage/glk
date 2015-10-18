#include <glk/gl/Instancer.h>

namespace glk {
	namespace gl {
		template <class AttrDesc>
		template <class InputIter, class>
		Instancer<AttrDesc>::Instancer(InputIter vertBeg, InputIter vertEnd)
			: _verticeCount(std::distance(vertBeg, vertEnd)) {
			TRY_GL(glGenBuffers(1, &_vertexVbo));
			TRY_GL(glBindBuffer(GL_ARRAY_BUFFER, _vertexVbo));

			TRY_GL(glBufferData(GL_ARRAY_BUFFER, _verticeCount * sizeof(*vertBeg), std::addressof(*vertBeg), GL_STATIC_DRAW));

			TRY_GL(glBindBuffer(GL_ARRAY_BUFFER, 0u));
		}

		template <class AttrDesc>
		Instancer<AttrDesc>::Instancer(gl::Instancer<AttrDesc> &&other)
			: _vertexVbo{other._vertexVbo}
			, _verticeCount{other._verticeCount} {
			other._vertexVbo = 0u;
			other._verticeCount = 0u;
		}

		template <class AttrDesc>
		Instancer<AttrDesc>::~Instancer() {
			TRY_GL(glDeleteBuffers(1, &_vertexVbo));
		}

		template <class AttrDesc>
		GLuint Instancer<AttrDesc>::verticeCount() const {
			return _verticeCount;
		}

		template <class AttrDesc>
		GLuint Instancer<AttrDesc>::vertexVbo() const {
			return _vertexVbo;
		}

		template <class AttrDesc>
		InstancerQueue<AttrDesc> Instancer<AttrDesc>::makeQueue() const {
			return {*this};
		}

		template <class AttrDesc>
		InstancerQueue<AttrDesc>::InstancerQueue(Instancer<AttrDesc> const &instancer)
			: _instancer{instancer}
			, _capacity{0u} {
			TRY_GL(glGenVertexArrays(1, &_attrVao));
			TRY_GL(glBindVertexArray(_attrVao));

			// Vetex data pointer
			TRY_GL(glBindBuffer(GL_ARRAY_BUFFER, _instancer.vertexVbo()));
			{
				GLuint locIdx = 0u;
				detail_Instancer::RegAttr<typename AttrDesc::VertexType>::reg(locIdx, 0, 0);
				TRY_GL(glVertexAttribDivisor(0u, 0));
			}

			// Set the attributes VBO as provider for other varyings
			TRY_GL(glGenBuffers(1, &_attrVbo));
			TRY_GL(glBindBuffer(GL_ARRAY_BUFFER, _attrVbo));
			AttrDesc::setAttribPointers();

			TRY_GL(glBindVertexArray(0u));
		}

		template <class AttrDesc>
		InstancerQueue<AttrDesc>::InstancerQueue(InstancerQueue &&other)
			: _instancer{other._instancer}
			, _attrVao{other._attrVao}
			, _attrVbo{other._attrVbo}
			, _attribs{std::move(other._attribs)}
			, _capacity{other._capacity} {
			other._attrVao = 0u;
			other._attrVbo = 0u;
			other._capacity = 0u;
		}

		template <class AttrDesc>
		InstancerQueue<AttrDesc>::~InstancerQueue() {
			TRY_GL(glDeleteVertexArrays(1, &_attrVbo));
			TRY_GL(glDeleteBuffers(1, &_attrVao));
		}

		template <class AttrDesc>
		void InstancerQueue<AttrDesc>::enqueue(AttrDesc const &attribs) {
			_attribs.push_back(attribs);
		}

		template <class AttrDesc>
		void InstancerQueue<AttrDesc>::clear() {
			_attribs.clear();
		}

		template <class AttrDesc>
		void InstancerQueue<AttrDesc>::uploadAttribs() {
			TRY_GL(glBindVertexArray(_attrVao));

			if(_attribs.capacity() > _capacity) {
				_capacity = _attribs.capacity();
				TRY_GL(glBufferData(GL_ARRAY_BUFFER, _capacity * sizeof(AttrDesc), _attribs.data(), GL_DYNAMIC_DRAW));
			} else
				TRY_GL(glBufferSubData(GL_ARRAY_BUFFER, 0u, _attribs.size() * sizeof(AttrDesc), _attribs.data()));
		}

		template <class AttrDesc>
		void InstancerQueue<AttrDesc>::display() {
			TRY_GL(glBindVertexArray(_attrVao));
			TRY_GL(glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, _instancer.verticeCount(), _attribs.size()));
		}

	}
}