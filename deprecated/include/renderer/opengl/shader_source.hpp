#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include "renderer/shader_source.hpp"
#include "renderer/opengl/opengl.hpp"

namespace marathon {

namespace renderer {

namespace opengl {

class ShaderSource : public renderer::ShaderSource {
private:
    static const std::map<ShaderStage, GLenum> _stageToGL;

public:
    ShaderSource(const std::string& name="Default Shader Stage", const std::string& source="", ShaderStage stage=ShaderStage::INVALID);

    uint32_t Compile();
};

} // opengl

} // renderer

} // marathon