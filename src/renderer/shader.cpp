#include "renderer/shader.hpp"
#include "renderer/opengl/opengl_shader.hpp"

namespace marathon {

namespace renderer {

// set default active shader to null
std::shared_ptr<Shader> Shader::active = nullptr;

void Shader::Bind() {
    active = std::static_pointer_cast<Shader>(shared_from_this());
}

void Shader::Unbind() {
    active = nullptr;
}

std::shared_ptr<Shader> Shader::Create(const std::string& name, std::shared_ptr<ShaderSource> vs, std::shared_ptr<ShaderSource> fs) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::NONE:
            return nullptr;
        case RendererAPI::API::OPENGL:
            return std::make_shared<opengl::OpenGLShader>(name, vs, fs);
        default:
            throw std::runtime_error("Unknown Graphics API");
    }
}

} // renderer

} // marathon