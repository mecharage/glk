#pragma once

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

namespace glk {
	namespace gl {
		namespace detail {
			template <class T>
			struct access;

			template <>	struct access<float> {
				static constexpr auto const &set = glUniform1f;
			} ;

			template <>	struct access<int> {
				static constexpr auto const &set = glUniform1i;
			} ;

			template <>	struct access<unsigned int> {
				static constexpr auto const &set = glUniform1ui;
			} ;

			template <>	struct access<glm::mat3> {

				static void set(GLint location, glm::mat3 const &mat) {
					glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
				};
			} ;

			template <>	struct access<glm::mat4> {

				static void set(GLint location, glm::mat4 const &mat) {
					glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
				};
			} ;
		}
	}
}