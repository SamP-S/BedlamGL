#pragma once

#include <string>
#include <cstdint>
#include <map>
#include <iostream>
#include <variant>

#include "renderer/material.hpp"

namespace marathon {

namespace renderer {

namespace opengl {

class OpenGLMaterial : public Material {
public:
    OpenGLMaterial(std::string name = "Material");

    bool IsUsable() const;
    void Bind();
    void Unbind();
};

} // opengl

} // renderer

} // marathon