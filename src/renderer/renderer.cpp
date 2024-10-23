#include "renderer/renderer.hpp"
#include "renderer/opengl/renderer.hpp"

namespace marathon {

namespace renderer {

Renderer::Render(const std::string& name)
    : Module(ModuleType::RENDERER, name) {}

Renderer& Renderer::Instance() {
    static Renderer* instance;
    if (!instance)
        instance = new marathon::renderer::opengl::Renderer();
    return *instance;
}

} // renderer

} // marathon