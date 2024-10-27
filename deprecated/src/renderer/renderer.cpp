#include "renderer/renderer.hpp"
#include "renderer/opengl/opengl_renderer.hpp"

namespace marathon {

namespace renderer {

Renderer& Renderer::Instance() {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::NONE:
            throw std::runtime_error("No Graphics API");
        case RendererAPI::API::OPENGL:
            return opengl::OpenGLRenderer::Instance();
        default:
            throw std::runtime_error("Unknown Graphics API");
    }
}

} // renderer

} // marathon
