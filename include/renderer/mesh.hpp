#pragma once

#include <vector>
#include <iostream>
#include <string>

#include "la_extended.h"
#include "renderer/drawable.hpp"
#include "renderer/renderer_api.hpp"

namespace marathon {

namespace renderer {

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
