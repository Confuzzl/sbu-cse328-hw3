cmake_minimum_required(VERSION 3.16)
project(hw3)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)

add_executable(hw3 
  "src/util.cpp"
  "src/color.cpp"
  "src/main.cpp"
  "src/app/app.cpp"
  "src/gl/uniform.cpp"
  "src/gl/gl_object.cpp"
  "src/gl/glsl_object.cpp"
  "src/callback.cpp"
  "src/gl/texture.cpp"
  "src/gl/vertex_layout.cpp"
  "src/input/key.cpp"
  "src/input/input_handler.cpp"
  "include/stb_image.h"
  "include/gl/gl_object.h"
  "include/gl/glsl_object.h"
  "include/gl/uniform.h"
  "include/gl/vao.h"
  "include/gl/vertex_layout.h"
  "include/gl/component/vertex.h"
  "include/gl/component/fragment.h"
  "include/gl/component/tessellation.h"
  "include/gl/texture.h"
  "include/app/app.h"
  "include/input/key.h"
  "include/input/input_handler.h"
  "include/color.h"
  "include/callback.h"
  "include/gl/component/geometry.h"
  "include/app/base_scene.h"
  "include/app/base_world.h"
  "include/app/base_renderer.h"
  "src/app/base_renderer.cpp"
  "include/app/all_scenes.h"
  "include/app/scene1.h"
  "include/app/scene2.h"
  "include/app/scene3.h"
  "include/app/scene4.h"
  "include/app/scene5.h"
  "include/app/scene6.h"
  "include/app/scene7.h"
  "src/app/base_world.cpp"
  "src/app/scene1.cpp"
  "src/app/scene2.cpp"
  "src/app/scene3.cpp"
  "src/app/scene4.cpp"
  "src/app/scene5.cpp"
  "src/app/scene6.cpp"
  "src/app/scene7.cpp"
  "include/geom/triangle.h"
  "src/geom/triangle.cpp"
  "include/geom/shape.h"
  "include/app/scene1.h"
  "src/app/scene1.cpp" "src/app/base_scene.cpp" "include/geom/rotator.h" "src/geom/rotator.cpp" "include/app/scene8.h" "src/app/scene8.cpp")
target_include_directories(hw3 PUBLIC include)
target_compile_definitions(hw3 PRIVATE SOURCE_DIR="${CMAKE_SOURCE_DIR}")

if (WIN32)
  list(APPEND CMAKE_PREFIX_PATH "C:/Program Files (x86)/GLFW/lib/cmake")
endif()
find_package(glfw3 REQUIRED)
target_link_libraries(hw3 glfw)

add_subdirectory("glad")
target_link_libraries(hw3 glad)

add_subdirectory("glm")
target_link_libraries(hw3 glm::glm)

add_subdirectory("imgui")
target_link_libraries(hw3 imgui)

add_subdirectory("fmt")
target_link_libraries(hw3 fmt::fmt)

if (CMAKE_DL_LIBS)
  target_link_libraries(hw3 ${CMAKE_DL_LIBS})
endif()