// fragr entry point — beat 3: read a shader file, wrap it, and compile it on
// the GPU. Report success, or print the compiler's error and exit non-zero.

#include "gl_context.h"
#include "shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "usage: fragr <file.frag>\n";
        return 1;
    }

    // Read the whole file into a string (Python's open(path).read()).
    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "fragr: cannot open " << argv[1] << '\n';
        return 1;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string user_source = buffer.str();

    try {
        fragr::GlContext gl;  // a current GL context is required to compile anything
        std::string wrapped = fragr::wrap_shadertoy(user_source);
        fragr::Shader frag = fragr::compile_fragment(wrapped);
        std::cout << "fragr: " << argv[1] << " compiled OK (shader id "
                  << frag.id() << ")\n";
    } catch (const fragr::ShaderError& e) {
        // A user shader with a mistake: show the compiler log verbatim. Its line
        // numbers point at the user's file thanks to #line.
        std::cerr << "fragr: could not compile " << argv[1] << ":\n\n"
                  << e.what() << '\n';
        return 1;
    } catch (const std::exception& e) {
        // Anything else, e.g. failing to obtain a GL context.
        std::cerr << "fragr: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
