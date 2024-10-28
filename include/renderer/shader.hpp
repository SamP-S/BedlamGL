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

enum class ShaderType {
    VERTEX,
    FRAGMENT
};

class Shader : public Resource {
protected:
    std::string _vSrc = "";
    std::string _fSrc = "";

    Shader(const std::string& name, const std::string& vSrc, const std::string& fSrc);
    
public:
    virtual ~Shader();

    /// TODO: bind/unbind should not be public
    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual std::string GetWarnings() const = 0;
    virtual bool HasUniform(const std::string& key) const = 0;

    // single value uniforms
    virtual bool SetUniform(const std::string& key, bool value) const = 0;
    virtual bool SetUniform(const std::string& key, int value) const = 0;
    virtual bool SetUniform(const std::string& key, uint32_t value) const = 0;
    virtual bool SetUniform(const std::string& key, float value) const = 0;
    virtual bool SetUniform(const std::string& key, double value) const = 0;
    // vector uniforms
    virtual void SetUniform(const std::string& key, const LA::vec2& v) const = 0;
    virtual void SetUniform(const std::string& key, float x, float y) const = 0;
    virtual void SetUniform(const std::string& key, const LA::vec3& v) const = 0;
    virtual void SetUniform(const std::string& key, float x, float y, float z) const = 0;
    virtual void SetUniform(const std::string& key, const LA::vec4& v) const = 0;
    virtual void SetUniform(const std::string& key, float x, float y, float z, float w) const = 0;
    // matrix uniforms
    virtual void SetUniform(const std::string& key, const LA::mat2& m) const = 0;
    virtual void SetUniform(const std::string& key, const LA::mat3& m) const = 0;
    virtual void SetUniform(const std::string& key, const LA::mat4& m) const = 0;
};

}

}