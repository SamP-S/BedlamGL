#include "renderer/mesh.hpp"
#include "renderer/opengl/opengl_mesh.hpp"

namespace marathon {

namespace renderer {

std::shared_ptr<Mesh> Mesh::Create(std::string name) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::API::NONE:   
            return nullptr;
        case RendererAPI::API::OPENGL:
            return std::make_shared<opengl::OpenGLMesh>(name);
        default:
            throw std::runtime_error("Unknown Graphics API");
    }
}
    
} // renderer

} // marathon