#include "renderer/shader_source.hpp"
#include "renderer/opengl/opengl_shader_source.hpp"

namespace marathon {

namespace renderer {

std::shared_ptr<ShaderSource> ShaderSource::Create(const std::string& name, const std::string& source, const ShaderStage& stage) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::NONE:
            return nullptr;
        case RendererAPI::API::OPENGL:
            return std::make_shared<opengl::OpenGLShaderSource>(name, source, stage);
        default:
            throw std::runtime_error("Unknown Graphics API");
    }
}

} // renderer

} // marathon