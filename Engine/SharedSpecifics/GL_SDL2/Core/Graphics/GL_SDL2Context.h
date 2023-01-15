#pragma once

namespace GL_SDL2 {
    // This function inits an OpenGL context using SDL2 functions.
    // This function must be reimplemented on all platforms using both dependencies
    // as initialisation process differs.
    bool InitContext();
}