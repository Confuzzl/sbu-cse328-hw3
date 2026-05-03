#include "gl/vertex_layout.h"

namespace shaders::vertex_layout {
template <> void enable<pos>(const GLuint vaoID) { enable<glm::vec3>(vaoID); }
template <> void enable<postex>(const GLuint vaoID) {
  enable<glm::vec3, glm::vec2>(vaoID);
}
template <> void enable<posnorm>(const GLuint vaoID) {
  enable<glm::vec3, glm::vec3>(vaoID);
}
template <> void enable<sphere>(const GLuint vaoID) {
  enable<glm::vec3, glm::vec1>(vaoID);
}
template <> void enable<cylinder>(const GLuint vaoID) {
  enable<glm::vec3, glm::vec1, glm::vec1>(vaoID);
}
template <> void enable<superquadric>(const GLuint vaoID) {
  enable<glm::vec3, glm::vec3, glm::vec3>(vaoID);
}
} // namespace shaders::vertex_layout