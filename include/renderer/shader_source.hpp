#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "core/resource.hpp"
#include "renderer/renderer_api.hpp"

namespace marathon {

namespace renderer {

static const std::string defaultVertexShader = R"(
#version 330 core

// // standard variables
// in int gl_VertexID;
// in int gl_InstanceID;
// in int gl_DrawID; // Requires GLSL 4.60 or ARB_shader_draw_parameters
// in int gl_BaseVertex; // Requires GLSL 4.60 or ARB_shader_draw_parameters
// in int gl_BaseInstance; // Requires GLSL 4.60 or ARB_shader_draw_parameters
// out vec4 gl_Position;
// out int gl_PointSize;

// vertex attributes
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec4 aTangent;
layout(location = 3) in vec4 aColour;
layout(location = 4) in vec2 aUV0;
layout(location = 5) in vec2 aUV1;
layout(location = 6) in vec2 aUV2;
layout(location = 7) in vec2 aUV3;

// "varying" variables
out vec3 vColour;

// uniforms
uniform float   uTime;
uniform mat4	uTransform;

void main()
{
    vColour = aPosition;
	gl_Position = uTransform * vec4(aPosition, 1.0f);
}
)";

static const std::string defaultFragmentShader = R"(
#version 330 core

// // standard variables
// in vec4 gl_FragCoord;
// in bool gl_FrontFacing;
// in vec2 gl_PointCoord;
// out float gl_FragDepth;

// "varying" variables
in vec3 vColour;

// uniforms
uniform vec3    uResolution;
uniform float   uTime;
uniform float   uTimeDelta;
uniform int     uFrame;  

// outputs
out vec4 oColour;

void main()
{
    // oColour = vec4(vColour, 1);
    oColour = vec4(1, 1, 1, 1);
}
)";

enum class ShaderStage {
    INVALID = 0,
    VERTEX = 1,
    FRAGMENT = 2,
    COMPUTE = 3,
    GEOMETRY = 4,
    TESSELLATION_CONTROL = 5,
    TESSELLATION_EVALUATION = 6
};

class ShaderSource : public Resource {
public:
    std::string source = "";
    ShaderStage stage = ShaderStage::INVALID;

    ShaderSource(const std::string& name="Default Shader Stage", const std::string& source="", const ShaderStage& stage=ShaderStage::INVALID)
        : Resource(name), source(source), stage(stage) {}

    virtual uint32_t Compile() = 0;

    static std::shared_ptr<ShaderSource> Create(const std::string& name, const std::string& source="", const ShaderStage& stage=ShaderStage::INVALID);

};

} // renderer

} // marathon
