#pragma once

#include <stdexcept>
#include <string>

namespace fragr {

// Thrown when a shader fails to compile; what() is the compiler's log.
class ShaderError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;  // reuse the (message) constructor
};

// Wraps a Shadertoy-style fragment (a file containing only a mainImage
// function) into a complete GLSL 410 core fragment shader: version directive,
// the uniforms fragr supplies, the user's code, and a main() entry point.
std::string wrap_shadertoy(const std::string& user_source);

// Owns a compiled OpenGL shader object (RAII). Construction compiles `source`;
// on failure it throws ShaderError carrying the compiler log (whose line
// numbers already point at the user's file, thanks to the #line directive).
class Shader {
public:
    Shader(unsigned int type, const std::string& source);  // type e.g. GL_FRAGMENT_SHADER
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    unsigned int id() const { return id_; }

private:
    unsigned int id_ = 0;
};

// Compiles `wrapped_source` as a fragment shader.
Shader compile_fragment(const std::string& wrapped_source);

}  // namespace fragr
