// fragr entry point: argument parsing and top-level orchestration.

#include "gl_context.h"

#include <iostream>

int main() {
    try {
        fragr::GlContext gl;
        std::cout << gl.describe();
    } catch (const std::exception& e) {
        std::cerr << "fragr: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
