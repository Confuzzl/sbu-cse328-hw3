#pragma once

#include "glad/gl.h"
#include <fmt/format.h>
#include <glm/glm.hpp>
#include <random>
#include <string_view>
#include <type_traits>

#if defined(_MSC_VER) && !defined(__clang__) // MSVC
#define UNREACHABLE __assume(false);
#else // GCC, Clang
#define UNREACHABLE __builtin_unreachable();
#endif

#define MOVE_ONLY_TYPE(type)                                                   \
  type(const type &) = delete;                                                 \
  type(type &&o) noexcept;                                                     \
  type &operator=(const type &) = delete;                                      \
  type &operator=(type &&o) noexcept;

void print_info(const std::string_view str);
void print_warn(const std::string_view str);
void print_err(const std::string_view str);

template <typename T = float> T random_float(const T a, const T b) {
  static std::random_device rd;
  static std::mt19937 mt{rd()};

  return std::uniform_real_distribution<T>{a, b}(mt);
}
template <typename T = int>
T random_int(const T a = std::numeric_limits<T>::min(),
             const T b = std::numeric_limits<T>::max()) {
  static_assert(std::is_integral_v<T>, "type must be integral");
  static std::random_device rd;
  static std::mt19937 mt{rd()};

  return std::uniform_int_distribution<T>{a, b}(mt);
}
glm::vec2 random_vec2();

glm::mat3 normal_matrix(const glm::mat3 &mat);

constexpr auto pow(const int a, const int n) {
  int out = 1;
  for (int i = 0; i < n; i++) {
    out *= a;
  }
  return out;
}

// C++20 ONLY
// template <std::size_t N> struct nttp_str {
//  char m[N];
//  constexpr nttp_str(const char (&s)[N]) { std::copy_n(s, N, m); }
//};

namespace GL {
std::string_view getErrorName();

template <typename T> constexpr GLenum macroOf();
template <> constexpr GLenum macroOf<GLbyte>() { return GL_BYTE; }
template <> constexpr GLenum macroOf<GLubyte>() { return GL_UNSIGNED_BYTE; }
template <> constexpr GLenum macroOf<GLshort>() { return GL_SHORT; }
template <> constexpr GLenum macroOf<GLushort>() { return GL_UNSIGNED_SHORT; }
template <> constexpr GLenum macroOf<GLint>() { return GL_INT; }
template <> constexpr GLenum macroOf<GLuint>() { return GL_UNSIGNED_INT; }
template <> constexpr GLenum macroOf<GLfloat>() { return GL_FLOAT; }
template <> constexpr GLenum macroOf<GLdouble>() { return GL_DOUBLE; }
} // namespace GL
