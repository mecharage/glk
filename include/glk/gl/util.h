#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <glk/util.h>
#include <glk/stringLiterals.h>

#define CHECK_GL_ERROR_(call)\
	do{\
		GLenum err = glGetError();\
		if(err)\
			throw glk::gl::GlException(glk::gl::getError(err), call, __FILE__, __LINE__);\
	} while(false)

#define CHECK_GL_ERROR()\
	CHECK_GL_ERROR_("<unknown>")

#define TRY_GL(call)\
	do{ (call); CHECK_GL_ERROR_(#call); } while(false)

namespace glk {
	namespace gl {
		inline namespace utils {

			struct GlException : std::exception {

				GlException(std::string const &error, std::string const &call, std::string const &file, int line)
				: _msg("Error : \""s + error + "\" from `" + call + "` [" + file + ":" + std::to_string(line) + "]") { }

				char const *what() const noexcept override {
					return _msg.c_str();
				}

			private:
				std::string _msg;
			} ;

			struct ShaderCompilationException : std::exception {

				ShaderCompilationException(std::string const &shaderSource, std::string const &errorLog)
				: _shaderSource(shaderSource), _errorLog(errorLog) { }

				char const *what() const noexcept override {
					return _errorLog.c_str();
				}

				std::string const &shaderSource() const {
					return _shaderSource;
				}

			private:
				std::string _shaderSource;
				std::string _errorLog;
			} ;

			struct ProgramLinkingException : std::exception {

				ProgramLinkingException(std::string const &errorLog)
				:  _errorLog(errorLog) { }

				char const *what() const noexcept override {
					return _errorLog.c_str();
				}

			private:
				std::string _errorLog;
			} ;

			inline std::string getError(GLenum error) {
				GLubyte const *str = gluErrorString(error);
				return reinterpret_cast<char const*> (str);
			}

			inline std::string getError() {
				return getError(glGetError());
			}

			inline std::string shaderLog(GLuint shaderId) {
				int logLength;
				TRY_GL(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength));
				std::string str;
				str.resize(logLength);
				TRY_GL(glGetShaderInfoLog(shaderId, logLength, NULL, &str.front()));
				return str;
			}

			inline std::string programLog(GLuint programId) {
				int logLength;
				TRY_GL(glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength));
				std::string str;
				str.resize(logLength);
				TRY_GL(glGetProgramInfoLog(programId, logLength, NULL, &str.front()));
				return str;
			}

			inline glm::mat3 rotate(float angle) {
				angle = glk::deg2rad(angle);
				float s = std::sin(angle);
				float c = std::cos(angle);

				return glm::mat3(
					c, -s, 0.0f,
					s, c, 0.0f,
					0.0f, 0.0f, 1.0f
					);
			}

			inline glm::mat3 translate(glm::vec2 const &t) {
				return glm::mat3(
					1.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f,
					t.x, t.y, 1.0f
					);
			}

			inline glm::mat3 rotate(float angle, glm::vec2 const &center) {
				return translate(center) * rotate(angle) * translate(-center);
			}
		}
	}
}
