#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>

#include <type_traits>

#include "util.h"

namespace shaders::vertex_layout {
namespace detail {
// template <typename T>
// void attribFormatDispatch(const GLuint vaoID, const GLuint attrIndex,
//                           const GLint size, const GLenum type,
//                           const GLint offset);
// template <typename Integer,
//          std::enable_if_t<std::is_integral_v<Integer>, bool> = true>
// void attribFormatDispatch(const GLuint vaoID, const GLuint attrIndex,
//                          const GLint size, const GLenum type,
//                          const GLint offset) {
//  static_assert(false, "doesnt seem to work");
//  glVertexArrayAttribIFormat(vaoID, attrIndex, size, type, offset);
//}
template <typename Float,
          std::enable_if_t<std::is_same_v<Float, GLfloat>, bool> = true>
void attribFormatDispatch(const GLuint vaoID, const GLuint attrIndex,
                          const GLint size, const GLenum type,
                          const GLint offset) {
  glVertexArrayAttribFormat(vaoID, attrIndex, size, type, false, offset);
}
// template <typename Double,
//           std::enable_if_t<std::is_same_v<Double, GLdouble>, bool> = true>
// void attribFormatDispatch(const GLuint vaoID, const GLuint attrIndex,
//                           const GLint size, const GLenum type,
//                           const GLint offset) {
//   glVertexArrayAttribLFormat(vaoID, attrIndex, size, type, offset);
// }

template <typename T>
void enable_helper(const GLuint vaoID, GLuint &attrIndex, GLint &offset) {
  glEnableVertexArrayAttrib(vaoID, attrIndex);
  attribFormatDispatch<typename T::value_type>(
      vaoID, attrIndex, T::length(), GL::macroOf<typename T::value_type>(),
      offset);
  glVertexArrayAttribBinding(vaoID, attrIndex, 0);
  attrIndex++;
  offset += sizeof(T);
}
} // namespace detail

template <typename... VertexTypes> void enable(const GLuint vaoID) {
  GLuint attrIndex = 0;
  GLint offset = 0;
  (detail::enable_helper<VertexTypes>(vaoID, attrIndex, offset), ...);
}

struct alignas(4) pos {
  glm::vec3 _pos{};

  const void *data() const { return &_pos; }
};
template <> void enable<pos>(const GLuint vaoID);

struct alignas(4) postex {
  glm::vec3 pos{};
  glm::vec2 tex{};

  const void *data() const { return &pos; }
};
template <> void enable<postex>(const GLuint vaoID);

struct alignas(4) posnorm {
  glm::vec3 _pos{};
  glm::vec3 norm{};

  const void *data() const { return &_pos; }
};
template <> void enable<posnorm>(const GLuint vaoID);

struct alignas(4) sphere {
  glm::vec3 center{};
  float radius = 0;

  const void *data() const { return &center; }
};
template <> void enable<sphere>(const GLuint vaoID);

struct alignas(4) cylinder {
  glm::vec3 center{};
  float radius = 0;
  float height = 0;

  const void *data() const { return &center; }
};
template <> void enable<cylinder>(const GLuint vaoID);

using cone = cylinder;

using torus = cylinder;

struct alignas(4) superquadric {
  glm::vec3 center{};
  glm::vec3 scales{};
  glm::vec3 powers{};

  const void *data() const { return &center; }
};
template <> void enable<superquadric>(const GLuint vaoID);
} // namespace shaders::vertex_layout

namespace vert_lay = shaders::vertex_layout;