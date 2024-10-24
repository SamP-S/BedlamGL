#include "renderer/shader.hpp"

namespace marathon {

namespace renderer {

Shader::Shader(const std::string& name) 
    : Resource(name) {}
Shader::~Shader() {}

} // namespace renderer

} // namespace marathon