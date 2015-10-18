#pragma once

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/tuple/size.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/tuple/pop_back.hpp>

#include <glk/pp.h>
#include <glk/tmp.h>

namespace glk {
	namespace gl {
		namespace detail_Instancer {
			// Tag type denoting a vector-like object
			template <GLuint N>
			struct vec;

			// Attribute registerer
			template <class T>
			struct RegAttr {
				static_assert(!sizeof(T), "Unimplemented attribute registration");
			};

			// Attribute registerer for vector-like objects
			template <GLuint N>
			struct RegAttr<vec<N>> {
				static void reg(GLuint &locIdx, GLsizei stride, std::size_t offs) {
					TRY_GL(glEnableVertexAttribArray(locIdx));
					TRY_GL(glVertexAttribPointer(locIdx, N, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<GLvoid *>(offs)));
					TRY_GL(glVertexAttribDivisor(locIdx, 1));
					++locIdx;
				}

				static constexpr std::size_t const size = N * sizeof(float);
			};

			// Attribute registerer for array-like objects (matrices are handled as arrays of vectors)
			template <class T, GLuint N>
			struct RegAttr<T[N]> {
				static void reg(GLuint &locIdx, GLsizei stride, std::size_t offs) {
					for(GLuint i = 0u; i < N; ++i)
						RegAttr<T>::reg(locIdx, stride, offs + i * RegAttr<T>::size);
				}

				static constexpr std::size_t const size = N * RegAttr<T>::size;
			};

#define GLK_GL_DETAIL_ATTR_PTR_SYNONYM(type, syn)    \
        template <>                            \
        struct RegAttr<type> : RegAttr<syn> { }

			GLK_GL_DETAIL_ATTR_PTR_SYNONYM(float, vec<1>);

			GLK_GL_DETAIL_ATTR_PTR_SYNONYM(glm::vec2, vec<2>);
			GLK_GL_DETAIL_ATTR_PTR_SYNONYM(glm::vec3, vec<3>);
			GLK_GL_DETAIL_ATTR_PTR_SYNONYM(glm::vec4, vec<4>);

			GLK_GL_DETAIL_ATTR_PTR_SYNONYM(glm::mat2, vec<2>[2]);
			GLK_GL_DETAIL_ATTR_PTR_SYNONYM(glm::mat3, vec<3>[3]);
			GLK_GL_DETAIL_ATTR_PTR_SYNONYM(glm::mat4, vec<4>[4]);

#undef GLK_GL_DETAIL_ATTR_PTR_SYNONYM
		}
	}
}

// Extracts the type from a member tuple
// I.e. `(vec<int, 2>, foo)` becomes `vec<int, 2>`
#define GLK_DETAIL_INSTANCE_ATTRIB_MEMBER_TYPE(member)\
    GLK_PP_REMOVE_PARENS(BOOST_PP_TUPLE_POP_BACK(member))

// Extracts the name from a member tuple
// I.e. `(vec<int, 2>, foo)` becomes `foo`
#define GLK_DETAIL_INSTANCE_ATTRIB_MEMBER_NAME(member)\
    BOOST_PP_TUPLE_ELEM(\
        BOOST_PP_SUB(BOOST_PP_TUPLE_SIZE(member), 1),\
        member\
    )

// Declares the member described by a member tuple
// I.e. `(int, foo)` becomes `int foo;`
#define GLK_GL_DETAIL_DECLARE_INSTANCE_ATTRIB(r, structName, member)\
    GLK_DETAIL_INSTANCE_ATTRIB_MEMBER_TYPE(member)\
    GLK_DETAIL_INSTANCE_ATTRIB_MEMBER_NAME(member);

// Generates a call to `detail_Instancer::RegAttr` for `structName::member`
#define GLK_GL_DETAIL_SET_INSTANCE_ATTR_PTR(r, structName, member)\
    glk::gl::detail_Instancer::RegAttr<\
        GLK_DETAIL_INSTANCE_ATTRIB_MEMBER_TYPE(member)\
    >::reg(\
        locIdx,\
        sizeof(structName),\
        offsetof(\
            structName,\
            GLK_DETAIL_INSTANCE_ATTRIB_MEMBER_NAME(member)\
        )\
    );

// Defines the struct `name` with members declared from `members`,
// and a static setAttribPointers() function which sets attribute pointers
// for each member.
// `members` should be a double-parenthesised Boost.PP seq,
// i.e. `((int, foo))((float, bar))`
#define GLK_GL_DEFINE_INSTANCE_ATTRIBS_(name, vertexType, members)\
    struct name {\
        using VertexType = GLK_PP_REMOVE_PARENS(vertexType);\
        BOOST_PP_SEQ_FOR_EACH(\
            GLK_GL_DETAIL_DECLARE_INSTANCE_ATTRIB,\
            name,\
            members\
        )\
        static void setAttribPointers() {\
            GLuint locIdx = 1u;\
            BOOST_PP_SEQ_FOR_EACH(\
                GLK_GL_DETAIL_SET_INSTANCE_ATTR_PTR,\
                name,\
                members\
            )\
        }\
    };\
    static_assert(\
        std::is_standard_layout<name>{},\
        "Only standard-layout types may be used as instancing attribute packs."\
    )
