# THIS IS USING GLAD 2.0 GENERATOR WITH GL CORE 4.5

cmake_minimum_required(VERSION 3.16)
project(glad)
set(CMAKE_CXX_STANDARD 17)

add_library(glad
	"include/glad/gl.h"
	"include/KHR/khrplatform.h"	
	"src/gl.c"
)
target_include_directories(glad PUBLIC ${PROJECT_SOURCE_DIR}/include)