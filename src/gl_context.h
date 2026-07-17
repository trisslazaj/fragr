#pragma once

#include <string>

// Forward-declared so this header stays free of the GLFW include; callers do
// not take on GLFW as a transitive dependency.
struct GLFWwindow;

namespace fragr {

// Owns GLFW initialization and a hidden window providing an OpenGL 4.1 core
// context. Construction acquires these resources or throws std::runtime_error;
// destruction releases them.
class GlContext {
public:
    GlContext();
    ~GlContext();

    // Non-copyable: an instance uniquely owns its window and GLFW state.
    GlContext(const GlContext&) = delete;
    GlContext& operator=(const GlContext&) = delete;

    // Context vitals (vendor, renderer, GL version, GLSL version), one per line.
    std::string describe() const;

private:
    GLFWwindow* window_ = nullptr;
};

}  // namespace fragr
