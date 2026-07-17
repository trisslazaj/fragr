#include "gl_context.h"

// GLFW_INCLUDE_GLCOREARB selects <OpenGL/gl3.h> (the core-profile API) instead
// of the legacy OpenGL 1.x header GLFW would otherwise include on macOS.
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

#include <format>
#include <stdexcept>

namespace {

// Most recent GLFW error description, or a fallback when none is set.
std::string glfw_error() {
    const char* description = nullptr;
    glfwGetError(&description);
    return description ? description : "unknown GLFW error";
}

}  // namespace

namespace fragr {

GlContext::GlContext() {
    if (!glfwInit()) {
        throw std::runtime_error(std::format("GLFW init failed: {}", glfw_error()));
    }

    // macOS provides at most OpenGL 4.1, and only for a core-profile,
    // forward-compatible context; a less strict request yields 2.1.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    // The window backs the context only and is never shown; rendering targets
    // an offscreen framebuffer.
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    window_ = glfwCreateWindow(64, 64, "fragr", nullptr, nullptr);
    if (!window_) {
        // The destructor does not run for a constructor that throws, so the
        // preceding glfwInit() must be undone here before propagating.
        std::string reason = glfw_error();
        glfwTerminate();
        throw std::runtime_error(
            std::format("could not create an OpenGL 4.1 core context: {}", reason));
    }

    // GL calls require a context that is current on the calling thread.
    glfwMakeContextCurrent(window_);
}

GlContext::~GlContext() {
    glfwDestroyWindow(window_);
    glfwTerminate();
}

std::string GlContext::describe() const {
    auto gl_string = [](GLenum name) {
        return reinterpret_cast<const char*>(glGetString(name));
    };
    return std::format(
        "GL_VENDOR   : {}\n"
        "GL_RENDERER : {}\n"
        "GL_VERSION  : {}\n"
        "GLSL        : {}\n",
        gl_string(GL_VENDOR), gl_string(GL_RENDERER),
        gl_string(GL_VERSION), gl_string(GL_SHADING_LANGUAGE_VERSION));
}

}  // namespace fragr
