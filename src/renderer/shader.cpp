#include "renderer/shader.hpp"

namespace marathon {

namespace renderer {

Shader::Shader(const std::string& name, const std::string& vSrc, const std::string& fSrc) 
    : Resource(name), _vSrc(vSrc), _fSrc(fSrc) {}
Shader::~Shader() {}

} // namespace renderer

} // namespace marathon