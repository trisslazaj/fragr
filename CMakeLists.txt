cmake_minimum_required(VERSION 3.24)
project(fragr LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GLFW is fetched and built from source as part of our build, so the repo
# compiles on any machine with just CMake + a compiler — nothing to install.
include(FetchContent)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.4
)
# We only want the library, not GLFW's examples/tests/docs.
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

add_executable(fragr
  src/main.cpp
  src/gl_context.cpp
  src/shader.cpp
)

target_compile_options(fragr PRIVATE -Wall -Wextra)
target_link_libraries(fragr PRIVATE glfw)

if(APPLE)
  # macOS ships OpenGL 4.1 headers + symbols in a system framework, so no
  # loader library (glad) is needed here; glad enters when Linux support does.
  find_package(OpenGL REQUIRED)
  target_link_libraries(fragr PRIVATE OpenGL::GL)
  # Apple marks every GL function deprecated. We know. Quiet, please.
  target_compile_definitions(fragr PRIVATE GL_SILENCE_DEPRECATION)
endif()
