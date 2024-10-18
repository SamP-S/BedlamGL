#include "renderer/material.hpp"
#include "renderer/opengl/opengl_material.hpp"

namespace marathon {

namespace renderer {

std::shared_ptr<Material> Material::Create(const std::string& name) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::NONE:
            return nullptr;
        case RendererAPI::API::OPENGL:
            return std::make_shared<opengl::OpenGLMaterial>(name);
        default:
            throw std::runtime_error("Unknown Graphics API");
    }
}

} // renderer

} // marathon
