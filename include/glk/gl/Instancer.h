#pragma once

#include <glm/glm.hpp>

#include <GL/glew.h>

#include <glk/gl/util.h>
#include <glk/gl/detail_Instancer.h>

// Defines the struct `name` with members declared from `members`,
// and a static setAttribPointers() function which sets attribute pointers
// for each member.
// `members` should be a double-parenthesised Boost.PP seq,
// i.e. `((int, foo))((float, bar))`
#define GLK_GL_DEFINE_INSTANCE_ATTRIBS(name, vertexType, members)\
    GLK_GL_DEFINE_INSTANCE_ATTRIBS_(name, vertexType, GLK_PP_SEQ_DOUBLE_PARENS(members))

namespace glk {
	namespace gl {

		template <class>
		struct InstancerQueue;

		template <class AttrDesc>
		struct Instancer {
			//TODO : check that the iterators are ContiguousIterators (C++17)
			template <class InputIter, class = std::enable_if<std::is_same<
				typename std::iterator_traits<InputIter>::value_type, typename AttrDesc::VertexType
			>{}>>
			Instancer(InputIter vertBeg, InputIter vertEnd);

			Instancer(Instancer &&other);

			~Instancer();

			Instancer(Instancer const &) = delete;
			Instancer &operator =(Instancer const &) = delete;

			GLuint verticeCount() const;
			GLuint vertexVbo() const;

			InstancerQueue<AttrDesc> makeQueue() const;

		private:
			GLuint _vertexVbo;
			GLuint _verticeCount;
		};

		template <class AttrDesc>
		struct InstancerQueue {
			using VertexType = glm::vec2;

			InstancerQueue(Instancer<AttrDesc> const &instancer);
			InstancerQueue(InstancerQueue &&other);

			~InstancerQueue();

			InstancerQueue(InstancerQueue const &) = delete;
			InstancerQueue &operator = (InstancerQueue const &) = delete;

			void enqueue(AttrDesc const &attribs);

			void clear();

			void uploadAttribs();

			void display();

		private:
			Instancer<AttrDesc> const &_instancer;
			GLuint _attrVao, _attrVbo;
			std::vector<AttrDesc> _attribs;
			GLuint _capacity;
		};
	}
}

#include <glk/gl/Instancer.tpp>