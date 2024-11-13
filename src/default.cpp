#pragma once

#include <vector>
#include <string>
#include <utility>
#include <memory>
#include <cstdlib>
#include <unordered_map>

#include "la_extended.h"

namespace marathon {

namespace renderer {

/// NOTE: Default assets should be baked into code, as removes need for pathing and loading from external files
/// CONSIDER: Move these or hide them inside Renderer module

const std::vector<LA::vec3> defaultTriangleVertices = {
    {  0.0f,  1.0f, 0.0f },
    { -1.0f, -1.0f, 0.0f },
    {  1.0f, -1.0f, 0.0f }
};

const std::vector<LA::vec3> defaultQuadVertices = {
    { -1.0f, -1.0f, 0.0f },
    {  1.0f, -1.0f, 0.0f },
    {  1.0f,  1.0f, 0.0f },
    { -1.0f,  1.0f, 0.0f }
};
const std::vector<uint32_t> defaultQuadIndices = {
    0, 1, 2, 2, 3, 0
};

const std::vector<LA::vec3> defaultCubeVertices = {
    // front
    { -1.0f, -1.0f,  1.0f },
    {  1.0f, -1.0f,  1.0f },
    {  1.0f,  1.0f,  1.0f },
    { -1.0f,  1.0f,  1.0f },
    // back
    { -1.0f, -1.0f, -1.0f },
    {  1.0f, -1.0f, -1.0f },
    {  1.0f,  1.0f, -1.0f },
    { -1.0f,  1.0f, -1.0f }
};
const std::vector<uint32_t> defaultCubeIndices = {
    0, 1, 2, 2, 3, 0,
    1, 5, 6, 6, 2, 1,
    7, 6, 5, 5, 4, 7,
    4, 0, 3, 3, 7, 4,
    3, 2, 6, 6, 7, 3,
    4, 5, 1, 1, 0, 4
};

const std::string defaultVertexShader = R"(
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

const std::string defaultFragmentShader = R"(
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

} // namespace renderer

} // namespace marathon