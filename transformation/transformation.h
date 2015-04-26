#ifndef TRANSFORMATION
#define TRANSFORMATION

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#define M_PI	3.14159265358979323846264338327950288f   /* pi */

namespace Transformation
{
   glm::mat4 translate(const glm::vec3 &t);
   glm::mat4 rotate(GLfloat angle, glm::vec3 axis);
   glm::mat4 scale(glm::vec3 s);
}

#endif