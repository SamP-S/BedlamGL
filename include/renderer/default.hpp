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