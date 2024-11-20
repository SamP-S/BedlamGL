#include "renderer/material.hpp"

#include <iostream>
#include "renderer/renderer.hpp"

namespace marathon {

namespace renderer {



std::string ColourMaterial::_sVertexSource = R"(
void main()
{
    varying_position = vec4(vertex_position, 1.0f);
	gl_Position =  u_projection * u_view * u_model * vec4(vertex_position, 1.0f);
}
)";
std::string ColourMaterial::_sFragmentSource = R"(
// outputs
layout(location = 0) out vec4 out_color;

void main()
{
    out_color = varying_position;
    // out_color = vec4(1, 1, 1, 1);
}
)";

ColourMaterial::ColourMaterial() {
    _mShader = std::make_shared<Shader>();
    _mShader->SetSources(_sVertexSource, _sFragmentSource);
};
ColourMaterial::~ColourMaterial() {};

LA::vec4 ColourMaterial::GetColour() const {
    return _mColour;
}
void ColourMaterial::SetColour(LA::vec4 colour) {
    _mColour = colour;
}

} // renderer

} // marathon
