#pragma once

// PUBLIC HEADER

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

#include "la_extended.h"
#include "core/resource.hpp"

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

/// TODO:
// add support for seperated shader stages utilising defaults
// support a general default data layout for vertex attributes
// support a wider range of uniform sends

// enum class ShaderType {
//     VERTEX,
//     FRAGMENT
// };

/// TODO:
// shaders should determine what vertex attributes and uniforms are available
// shaders should use a standard layout
// custom mesh layouts should require custom shaders
// meshes (vertex attributes) must find the shader that matches their layout
// materials (uniforms) must match the shader's uniforms
// NOTE: unity uses multiple shader variances per possible missing vertex attribute and checks at runtime which to use according to mesh data available

enum class ShaderDirty {
    INVALID,
    DIRTY_RECOMPILE,
    CLEAN
};

class Shader : public Resource {
protected:
    std::string _vSrc = "";
    std::string _fSrc = "";
    ShaderDirty _dirty = ShaderDirty::INVALID;
 
public:
    Shader();
    ~Shader();

    std::string GetVertexSource() const;
    std::string GetFragmentSource() const;
    ShaderDirty GetDirtyFlag() const;

    void SetVertexSource(const std::string& vSrc);
    void SetFragmentSource(const std::string& fSrc);
    void SetSources(const std::string& vSrc, const std::string& fSrc);

};

}

}