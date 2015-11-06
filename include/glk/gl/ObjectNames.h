#pragma once

#include <GL/glew.h>

#include <glk/gl/util.h>

namespace glk {
	namespace gl {
		template <void (* const &Gen)(GLsizei, GLuint *), void (* const &Del)(GLsizei, GLuint const *)>
		struct ObjectName {
			ObjectName() {
				TRY_GL(Gen(1, &_name));
			}

			~ObjectName() {
				TRY_GL(Del(1, &_name));
			}
			
			ObjectName(ObjectName const &) = delete;
			ObjectName &operator =(ObjectName const &) = delete;

			ObjectName(ObjectName &&other)
				: _name{other._name} {
				other._name = 0u;
			}

			ObjectName &operator =(ObjectName &&rhs) {
				_name = rhs._name;
				rhs._name = 0u;
				return *this;
			}

			operator GLuint() const {
				return _name;
			}

		private:
			GLuint _name;
		};

		using VboName = ObjectName<glGenBuffers, glDeleteBuffers>;
		using VaoName = ObjectName<glGenVertexArrays, glDeleteVertexArrays>;
	}
}
