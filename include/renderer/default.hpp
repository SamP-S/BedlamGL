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

static const std::vector<LA::vec3> defaultTriangleVertices = {
    {  0.0f,  1.0f, 0.0f },
    { -1.0f, -1.0f, 0.0f },
    {  1.0f, -1.0f, 0.0f }
};

static const std::vector<LA::vec3> defaultQuadVertices = {
    { -1.0f, -1.0f, 0.0f },
    {  1.0f, -1.0f, 0.0f },
    {  1.0f,  1.0f, 0.0f },
    { -1.0f,  1.0f, 0.0f }
};
static const std::vector<uint32_t> defaultQuadIndices = {
    0, 1, 2, 2, 3, 0
};

static const std::vector<LA::vec3> defaultCubeVertices = {
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
static const std::vector<uint32_t> defaultCubeIndices = {
    0, 1, 2, 2, 3, 0,
    1, 5, 6, 6, 2, 1,
    7, 6, 5, 5, 4, 7,
    4, 0, 3, 3, 7, 4,
    3, 2, 6, 6, 7, 3,
    4, 5, 1, 1, 0, 4
};



static const std::string defaultVertexShader = R"(
void main()
{
    varying_position = vec4(vertex_position, 1.0f);
	gl_Position = u_model_view_projection * vec4(vertex_position, 1.0f);
}
)";

static const std::string defaultFragmentShader = R"(
// outputs
out vec4 out_color;

void main()
{
    out_color = varying_position;
    // out_color = vec4(1, 1, 1, 1);
}
)";

} // namespace renderer

} // namespace marathon