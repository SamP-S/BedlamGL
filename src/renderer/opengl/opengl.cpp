#include "renderer/opengl/opengl.hpp"

namespace marathon {

namespace renderer {

namespace opengl {

bool CheckError() {
    uint32_t error = glGetError();
    if (error == GL_NO_ERROR) {
        return true;
    }
    
    std::cout << "ERROR (OpenGL): ";
    switch (error) {
        case GL_INVALID_OPERATION:
            std::cout << "operation is not allowed in the current state" << std::endl;
            break;
        case GL_INVALID_VALUE:
            std::cout << "numeric argument is out of range" << std::endl;
            break;
        case GL_STACK_OVERFLOW:
            std::cout << "stack overflow" << std::endl;
            break;
        case GL_STACK_UNDERFLOW:
            std::cout << "unacceptable value for an enum arg" << std::endl;
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            std::cout << "framebuffer invalid or not complete" << std::endl;
            break;
        case GL_OUT_OF_MEMORY:
            std::cout << "out of GPU memory" << std::endl;
            break;
        case GL_INVALID_ENUM:
            std::cout << "unacceptable value for an enum arg" << std::endl;
            break;
        default:
            std::cout << "unknown error" << std::endl;
            break;
    }
    return false;
}

bool CheckFrameBufferError() {
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status == GL_FRAMEBUFFER_COMPLETE) {
        return true;
    }
    
    std::cout << "ERROR (OpenGL): ";
    switch (status) {
        case GL_FRAMEBUFFER_UNDEFINED:
            std::cout << "framebuffer undefined" << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            std::cout << "framebuffer incomplete attachment" << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            std::cout << "framebuffer incomplete missing attachment" << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            std::cout << "framebuffer incomplete draw buffer" << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            std::cout << "framebuffer incomplete read buffer" << std::endl;
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            std::cout << "framebuffer unsupported" << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
            std::cout << "framebuffer incomplete multisample" << std::endl;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
            std::cout << "framebuffer incomplete layer targets" << std::endl;
            break;
        default:
            std::cout << "unknown error" << std::endl;
            break;
    }
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
    std::cout << "ERROR (OpenGL): shader compilation failed\n" << infoLog << std::endl;
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
    std::cout << "ERROR (OpenGL): program linking failed\n" << infoLog << std::endl;
    return false;
}
bool IsEnabled(GLenum cap) {
    return glIsEnabled(cap) == GL_TRUE;
}

} // opengl

} // renderer

} // marathon