#pragma once

#include <GL/glew.h>

#include <glk/gl/util.h>

namespace glk {
	namespace gl {
#define GLK_DETAIL_DEFINE_GL_OBJECT(type, genFunc, delFunc)\
    struct type {\
    type(){\
        TRY_GL(genFunc(1, &_name));\
    }\
    \
    ~type(){\
        TRY_GL(delFunc(1, &_name));\
    }\
    \
    type(type const &) = delete;\
    type &operator = (type const &) = delete;\
    \
    type(type &&other)\
        : _name{other._name} {\
        other._name = 0u;\
    }\
    \
    type &operator = (type &&rhs) {\
        _name = rhs._name;\
        rhs._name = 0u;\
		return *this;\
    }\
    \
    GLuint name() const {\
        return _name;\
    }\
    \
    private:\
        GLuint _name;\
    }

		GLK_DETAIL_DEFINE_GL_OBJECT(Vbo, glGenBuffers, glDeleteBuffers);

		GLK_DETAIL_DEFINE_GL_OBJECT(Vao, glGenVertexArrays, glDeleteVertexArrays);
	}
}
