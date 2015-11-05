#pragma once

#include <glm/glm.hpp>

#include <GL/glew.h>

#include <glk/gl/util.h>
#include <glk/gl/GlObjects.h>
#include <glk/gl/detail/Instancer.h>

#define GLK_GL_ATTRIB_STRUCT(name, members)\
    GLK_GL_ATTRIB_STRUCT_(name, GLK_PP_SEQ_DOUBLE_PARENS(members))

namespace glk {
	namespace gl {
		template <class T>
		GLuint setAttribPointers(GLuint locIdx = 0u);

		template <class Vertex, class Attrib>
		struct InstanceQueue;

		template <class Vertex>
		struct Instancer {
			//TODO : check that the iterators are ContiguousIterators (C++17)
			template <class InputIter, class = std::enable_if<std::is_same<
				typename std::iterator_traits<InputIter>::value_type, Vertex
			>{}>>
			Instancer(InputIter vertBeg, InputIter vertEnd);
			Instancer(Instancer &&other) = default;

			~Instancer() = default;

			Instancer &operator =(Instancer &&rhs) = default;

			Instancer(Instancer const &) = delete;
			Instancer &operator =(Instancer const &) = delete;

			GLuint verticeCount() const;
			GLuint vertexVboName() const;

			template <class Attrib>
			InstanceQueue<Vertex, Attrib> makeQueue() const;

		private:
			Vbo _vertexVbo;
			GLuint _verticeCount;
		};

		template <class Vertex, class Attrib>
		struct InstanceQueue {
			using VertexType = glm::vec2;

			InstanceQueue(Instancer<Vertex> const &instancer);
			InstanceQueue(InstanceQueue &&other);

			~InstanceQueue() = default;

			InstanceQueue(InstanceQueue const &) = delete;
			InstanceQueue &operator =(InstanceQueue const &) = delete;
			InstanceQueue &operator =(InstanceQueue &&rhs) = delete;

			void enqueue(Attrib const &attribs);

			void upload();
			void display();
			void clear();
			void displayAndClear();

		private:
			Instancer<Vertex> const &_instancer;
			Vbo _attrVbo;
			Vao _attrVao;
			std::vector<Attrib> _attribs;
			GLuint _capacity;
			bool _dirty;
		};
	}
}

#include <glk/gl/Instancer.tpp>