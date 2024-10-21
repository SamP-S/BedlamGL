#pragma once

#include <vector>
#include <iostream>
#include <string>

#include "la_extended.h"
#include "renderer/drawable.hpp"
#include "renderer/renderer_api.hpp"

namespace marathon {

namespace renderer {

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

class Renderer;

//// TODO:
// add sub mesh support for future more complex meshes
// each sub-mesh can have its own material but ONLY 1 material
// multiple materials for a submesh need multiple sub-meshes
// support more texture channels
// add internal state to ensure recompilation if vertices changed

class Mesh : public Drawable {
public:
    std::vector<LA::vec3> vertices;
    std::vector<LA::vec3> normals;
    std::vector<LA::vec4> tangents;
    std::vector<LA::vec4> colours;
    std::vector<uint32_t> indices;
    std::vector<LA::vec2> uv0;
    std::vector<LA::vec2> uv1;
    std::vector<LA::vec2> uv2;
    std::vector<LA::vec2> uv3;

    Mesh(const std::string name)
        : Drawable(name) {}

    virtual bool IsUsable() = 0;
    virtual void Draw(Renderer& renderer, const LA::mat4& m) = 0;

    static std::shared_ptr<Mesh> Create(std::string name);
};

} // renderer

} // marathon
