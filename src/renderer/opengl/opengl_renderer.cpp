#include "renderer/opengl/opengl_renderer.hpp"

namespace marathon {

namespace renderer {

namespace opengl {

void OpenGLRenderer::Boot() {
    std::cout << "DEBUG (OpenGLRenderer): Boot." << std::endl;
    std::cout << "DEBUG (OpenGLRenderer): OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << std::endl;
    }
    context = Context::Create();
}

void OpenGLRenderer::Shutdown() {
    std::cout << "DEBUG (OpenGLRenderer): Shutdown." << std::endl;
}

void OpenGLRenderer::SetFrameBuffer(std::shared_ptr<FrameBuffer> fb) {
    _frameBuffer = fb;
    if (fb == nullptr)
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    else
        fb->Bind();
}

std::shared_ptr<FrameBuffer> OpenGLRenderer::GetFrameBuffer() {
    return _frameBuffer;
}

void OpenGLRenderer::SetProjection(const LA::mat4& proj) {
    _projection = proj;
}

LA::mat4 OpenGLRenderer::GetProjection() {
    return _projection;
}

void OpenGLRenderer::SetView(const LA::mat4& view) {
    _view = view;
}

LA::mat4 OpenGLRenderer::GetView() {
    return _view;
}

void OpenGLRenderer::Draw(std::shared_ptr<Drawable> drawable, const LA::mat4& transform) {
    drawable->Draw(*this, transform);
}

void OpenGLRenderer::Clear() {
    if (_frameBuffer)
        _frameBuffer->Clear();
    else
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

Renderer& OpenGLRenderer::Instance() {
    static Renderer* instance;
    if (!instance)
        instance = new OpenGLRenderer();
    return *instance;
}

} // opengl

} // renderer

} // marathon