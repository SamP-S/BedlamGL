#pragma once

#include <iostream>

// include opengl deps
#define GL_VERSION_4_4
#include <GL/glew.h>

namespace marathon {

namespace renderer {

namespace opengl {

//// TODO:
// CONSIDER: should all opengl calls be wrapped into a single opengl class?
// This would allow render related fundamental objects to not require opengl calls
// effectively reducing backend to a single class.
// If not, should these methods be wrapped into the main renderer class?

bool CheckError();
bool CheckFrameBufferError();
bool CheckShaderError(GLuint shader);
bool CheckProgramError(GLuint program);
bool IsEnabled(GLenum cap);

} // opengl

} // renderer

} // marathon
