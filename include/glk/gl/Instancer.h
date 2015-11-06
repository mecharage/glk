#pragma once

#include <glm/glm.hpp>

#include <GL/glew.h>

#include <glk/gl/util.h>
#include <glk/gl/ObjectNames.h>
#include <glk/gl/Vbo.h>

#include <glk/gl/detail/Instancer.h>

#define GLK_GL_ATTRIB_STRUCT(name, members)\
    GLK_GL_ATTRIB_STRUCT_(name, GLK_PP_SEQ_DOUBLE_PARENS(members))

namespace glk {
	namespace gl {
		template <class T>
		GLuint setAttribPointers(GLuint locIdx = 0u);

		template <class Vertex, class Attrib>
		struct InstanceQueue {
			using VertexType = glm::vec2;

			InstanceQueue(Vbo<Vertex> const &vertices);

			InstanceQueue(InstanceQueue &&other) = default;
			InstanceQueue &operator =(InstanceQueue &&rhs) = default;

			InstanceQueue(InstanceQueue const &) = delete;
			InstanceQueue &operator =(InstanceQueue const &) = delete;

			void enqueue(Attrib const &attribs);

			void upload();
			void display();
			void clear();
			void displayAndClear();

		private:
			Vbo<Vertex> const &_vertVbo;
			Vbo<Attrib> _attrVbo;
			VaoName _attrVao;
			std::vector<Attrib> _attribs;
			GLuint _capacity;
			bool _dirty;
		};
	}
}

#include <glk/gl/Instancer.tpp>