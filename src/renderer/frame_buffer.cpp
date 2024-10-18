#include "renderer/frame_buffer.hpp"
#include "renderer/opengl/opengl_frame_buffer.hpp"

namespace marathon {

namespace renderer {

std::shared_ptr<FrameBuffer> FrameBuffer::Create(const std::string& name, int width, int height) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::NONE:
            return nullptr;
        case RendererAPI::API::OPENGL:
            return std::make_shared<opengl::OpenGLFrameBuffer>(name, width, height);
        default:
            throw std::runtime_error("Unknown Graphics API");
    }
}

} // renderer

} // marathon
