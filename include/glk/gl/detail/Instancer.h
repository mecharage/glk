#pragma once

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/tuple/size.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/tuple/pop_back.hpp>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <glk/pp.h>
#include <glk/tmp.h>
#include <glk/gl/util.h>

namespace glk {
	namespace gl {
		template <class T>
		struct attribTag_ { };

#define TAG_SYN(type, syn)\
    template <>\
    struct attribTag_<type> : attribTag_<syn> {}

#include <glk/gl/detail/glmTypes.h>

#undef TAG_SYN

		template <class T>
		using attribTag = attribTag_<T> const *;

		template <GLuint N>
		void setAttribPointers(GLuint &locIdx, GLsizei stride, std::intptr_t offs, attribTag<GLfloat[N]>) {
			TRY_GL(glVertexAttribPointer(locIdx, N, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid *>(offs)));
			++locIdx;
		}

		template <class T, GLuint N>
		void setAttribPointers(GLuint &locIdx, GLsizei stride, std::intptr_t offs, attribTag<T[N]>) {
			for(GLuint i = 0u; i < N; ++i)
				setAttribPointers(locIdx, stride, offs + i * sizeof(T), attribTag<T>{});
		}
	}
}

// Extracts the type from a member tuple
// I.e. `(vec<int, 2>, foo)` becomes `vec<int, 2>`
#define GLK_DETAIL_ATTRIB_MEMBER_TYPE(member)\
    GLK_PP_REMOVE_PARENS(BOOST_PP_TUPLE_POP_BACK(member))

// Extracts the name from a member tuple
// I.e. `(vec<int, 2>, foo)` becomes `foo`
#define GLK_DETAIL_ATTRIB_MEMBER_NAME(member)\
    BOOST_PP_TUPLE_ELEM(\
        BOOST_PP_SUB(BOOST_PP_TUPLE_SIZE(member), 1),\
        member\
    )

// Declares the member described by a member tuple
// I.e. `(int, foo)` becomes `int foo;`
#define GLK_GL_DETAIL_DECLARE_ATTRIB(r, structName, member)\
    GLK_DETAIL_ATTRIB_MEMBER_TYPE(member)\
    GLK_DETAIL_ATTRIB_MEMBER_NAME(member);

#define GLK_GL_DETAIL_SET_ATTRIB_PTRS(r, structName, member)\
    setAttribPointers(\
        locIdx,\
        stride,\
        offs + offsetof(\
            structName,\
            GLK_DETAIL_ATTRIB_MEMBER_NAME(member)\
        ),\
        glk::gl::attribTag<GLK_DETAIL_ATTRIB_MEMBER_TYPE(member)>{}\
    );

#define GLK_GL_ATTRIB_STRUCT_(name, members)\
    struct name {\
        BOOST_PP_SEQ_FOR_EACH(\
            GLK_GL_DETAIL_DECLARE_ATTRIB,\
            name,\
            members\
        )\
    };\
    inline void setAttribPointers(GLuint &locIdx, GLuint stride, std::intptr_t offs, glk::gl::attribTag<name>) {\
        using glk::gl::setAttribPointers;\
        BOOST_PP_SEQ_FOR_EACH(\
            GLK_GL_DETAIL_SET_ATTRIB_PTRS,\
            name,\
            members\
        )\
    }\
    static_assert(\
        std::is_standard_layout<name>{},\
        "Only standard-layout types may be used as instancing attribute packs."\
    )
