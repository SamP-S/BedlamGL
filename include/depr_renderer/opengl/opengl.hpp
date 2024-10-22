#pragma once

#include <iostream>

// include opengl deps
#define GL_VERSION_4_4
#include <GL/glew.h>

namespace marathon {

namespace renderer {

namespace opengl {

//// TODO:
// Move this to renderer/opengl context

bool OpenGLErrorCheck();

} // opengl

} // renderer

} // marathon
