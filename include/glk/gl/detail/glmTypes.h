// bit of help for the IDE
#ifndef TAG_SYN
#define TAG_SYN(...)
#include <glm/glm.hpp>
#endif

TAG_SYN(float, float[1]);

TAG_SYN(glm::vec2, GLfloat[2]);
TAG_SYN(glm::vec3, GLfloat[3]);
TAG_SYN(glm::vec4, GLfloat[4]);

TAG_SYN(glm::mat2, GLfloat[2][2]);
TAG_SYN(glm::mat3, GLfloat[3][3]);
TAG_SYN(glm::mat4, GLfloat[4][4]);

TAG_SYN(glm::ivec2, GLint[2]);
TAG_SYN(glm::ivec3, GLint[3]);
TAG_SYN(glm::ivec4, GLint[4]);
