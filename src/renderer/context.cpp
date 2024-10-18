#include "renderer/context.hpp"
#include "renderer/opengl/opengl_context.hpp"

std::shared_ptr<Context> Context::Create() {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::NONE:
            nullptr;
        case RendererAPI::API::OPENGL:
            return std::make_shared<OpenGLContext>();
        default:
            throw std::runtime_error("Unknown Graphics API");
    }
}
