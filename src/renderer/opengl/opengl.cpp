#include "renderer/opengl/opengl.hpp"

#include "core/logger.hpp"

namespace marathon {

namespace renderer {

namespace opengl {

bool CheckError() {
    uint32_t error = glGetError();
    if (error == GL_NO_ERROR) {
        return true;
    }
    
    std::string err = "";
    switch (error) {
        case GL_INVALID_OPERATION:
            err = "operation is not allowed in the current state.\n";
            break;
        case GL_INVALID_VALUE:
            err = "numeric argument is out of range.\n";
            break;
        case GL_STACK_OVERFLOW:
            err = "stack overflow.\n";
            break;
        case GL_STACK_UNDERFLOW:
            err = "unacceptable value for an enum arg.\n";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            err = "framebuffer invalid or not complete.\n";
            break;
        case GL_OUT_OF_MEMORY:
            err = "out of GPU memory.\n";
            break;
        case GL_INVALID_ENUM:
            err = "unacceptable value for an enum arg.\n";
            break;
        default:
            err = "unknown error.\n";
            break;
    }
    MT_CORE_ERROR("renderer::opengl::CheckError: {}", err);
    return false;
}

bool CheckFrameBufferError() {
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status == GL_FRAMEBUFFER_COMPLETE) {
        return true;
    }
    
    std::string err = "";
    switch (status) {
        case GL_FRAMEBUFFER_UNDEFINED:
            err = "framebuffer undefined.\n";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            err = "framebuffer incomplete attachment.\n";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            err = "framebuffer incomplete missing attachment.\n";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            err = "framebuffer incomplete draw buffer.\n";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            err = "framebuffer incomplete read buffer.\n";
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            err = "framebuffer unsupported.\n";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            err = "framebuffer incomplete multisample.\n";
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
            err = "framebuffer incomplete layer targets.\n";
            break;
        default:
            err = "unknown error.\n";
            break;
    }
    MT_CORE_ERROR("renderer::opengl::CheckFrameBufferError: {}", err);
    return false;
}
bool CheckShaderError(GLuint shader) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_TRUE) {
        return true;
    }
    
    char infoLog[512];
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    MT_CORE_ERROR("renderer::opengl::CheckShaderError: shader compilation failed\n{}", infoLog);
    return false;
}
bool CheckProgramError(GLuint program) {
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == GL_TRUE) {
        return true;
    }
    
    char infoLog[512];
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    MT_CORE_ERROR("renderer::opengl::CheckProgramError: program linking failed\n{}", infoLog);
    return false;
}
bool IsEnabled(GLenum cap) {
    return glIsEnabled(cap) == GL_TRUE;
}

} // opengl

} // renderer

} // marathon