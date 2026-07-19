#include "shader.h"

// The GL entry points for compiling shaders live behind GLFW's core header.
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>

namespace {

// Prepended before the user's code: version, the uniforms fragr supplies, and
// the shader's output variable (GL 4.1 core has no built-in gl_FragColor).
// R"(...)" is a raw string literal — like a Python triple-quoted string or a
// Java text block: newlines and quotes are taken verbatim, no escaping.
constexpr const char* kPrologue = R"(#version 410 core

uniform vec3  iResolution;  // viewport size in pixels; .z is pixel aspect (~1)
uniform float iTime;        // seconds elapsed; 0.0 for a single still frame

out vec4 fragrColor;
)";

// Appended after the user's code: the real entry point. gl_FragCoord is the
// built-in pixel coordinate, matching Shadertoy's fragCoord argument.
constexpr const char* kEpilogue = R"(
void main() {
    mainImage(fragrColor, gl_FragCoord.xy);
}
)";

}  // namespace

namespace fragr {

std::string wrap_shadertoy(const std::string& user_source) {
    return std::string(kPrologue) + "#line 1\n" + user_source + kEpilogue;
}

Shader::Shader(unsigned int type, const std::string& source) {
    id_ = glCreateShader(type);                 // allocate an empty shader object

    const char* src = source.c_str();           // GL takes an array of C strings;
    glShaderSource(id_, 1, &src, nullptr);      // we pass one, null-terminated
    glCompileShader(id_);                        // compile it now, in the GL driver

    GLint ok = GL_FALSE;
    glGetShaderiv(id_, GL_COMPILE_STATUS, &ok);  // did compilation succeed?
    if (ok != GL_TRUE) {
        GLint len = 0;
        glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &len);       // size of the error log
        std::string log(static_cast<size_t>(len), '\0');
        glGetShaderInfoLog(id_, len, nullptr, log.data());  // copy the log out
        glDeleteShader(id_);                     // the object is dead weight; free it
        id_ = 0;                                 // before we abandon this Shader
        throw ShaderError(log.empty() ? "shader compilation failed" : log);
    }
}

Shader::~Shader() {
    glDeleteShader(id_);   // glDeleteShader(0) is defined as a harmless no-op
}

Shader compile_fragment(const std::string& wrapped_source) {
    return Shader(GL_FRAGMENT_SHADER, wrapped_source);
}

}  // namespace fragr
