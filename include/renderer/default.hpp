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

static const std::vector<LA::vec3> defaultTriangleVertices;

static const std::vector<LA::vec3> defaultQuadVertices;
static const std::vector<uint32_t> defaultQuadIndices;

static const std::vector<LA::vec3> defaultCubeVertices;
static const std::vector<uint32_t> defaultCubeIndices;

static const std::string defaultVertexShader;
static const std::string defaultFragmentShader;

} // namespace renderer

} // namespace marathon