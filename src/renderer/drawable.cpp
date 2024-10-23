#include "renderer/drawable.hpp"

namespace marathon {

namespace renderer { 

Drawable::Drawable(const std::string& name)
    : Resource(name) {}

void Drawable::Draw(Renderer& renderer) {
    Draw(renderer, LA::mat4());
}

} // renderer

} // marathon
