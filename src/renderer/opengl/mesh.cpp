#include "renderer/opengl/mesh.hpp"
#include "renderer/opengl/buffer.hpp"

#include <cassert>

namespace marathon {

namespace renderer {

namespace opengl {



Mesh::Mesh(int vCount, size_t vSize, std::shared_ptr<renderer::Buffer> vBuf, std::vector<VertexAttribute> vAttrs, PrimitiveType primitive)
    : opengl::Mesh(vCount, vSize, vBuf, vAttrs, nullptr, IndexType::NONE, primitive) {
    assert(vBuf != nullptr && "Vertex buffer must not be null");
    assert(vCount > 0 && "Vertex count must be greater than 0");
    assert(vAttrs.size() > 0 && "Vertex attributes must not be empty");
}

Mesh::Mesh(int vCount, size_t vSize, std::shared_ptr<renderer::Buffer> vBuf, std::vector<VertexAttribute> vAttrs, std::shared_ptr<renderer::Buffer> iBuf, IndexType iType, PrimitiveType primitive)
    : renderer::Mesh("marathon.renderer.opengl.mesh", vCount, vSize, vBuf, vAttrs, iBuf, iType, primitive) {


    glGenVertexArrays(1, &_vao);
    Bind();

    vBuf->Bind();

    // auto openglVBuf = std::dynamic_pointer_cast<opengl::Buffer>(vBuf);
    // if (openglVBuf) {
    //     openglVBuf->Bind();
    // } else {
    //     throw std::runtime_error("renderer/opengl/mesh.cpp: Failed to cast vBuf to opengl::Buffer");
    // }

    for (const auto& attr : _vAttrs) {
        glEnableVertexAttribArray(attr.index);
        glVertexAttribPointer(attr.index, attr.numComponents, s_attrTypeMap.at(attr.type),
            attr.normalised, attr.stride, (void*)attr.offset);
    }

    if (_iBuffer != nullptr) {
        auto openglIBuf = std::dynamic_pointer_cast<opengl::Buffer>(_iBuffer);
        if (openglIBuf) {
            openglIBuf->Bind();
        } else {
            throw std::runtime_error("renderer/opengl/mesh.cpp: Failed to cast iBuf to opengl::Buffer");
        }
    }
    Unbind();

}
Mesh::~Mesh() {}

void Mesh::Bind() {
    assert(_vao != 0 && "VAO must be created before binding");
    glBindVertexArray(_vao);
}
void Mesh::Unbind() {
    glBindVertexArray(0);
}
void Mesh::Draw(renderer::Renderer& renderer) {
    assert(_vao != 0 && "VAO must be created before drawing");
    glBindVertexArray(_vao);
    if (_iBuffer != nullptr) {
        glDrawElements(s_primitiveMap.at(_primitive), _iBuffer->GetSize(), s_indexTypeMap.at(_indexType), nullptr);
    } else {
        glDrawArrays(s_primitiveMap.at(_primitive), 0, _vertexCount);
    }
}

void Mesh::SetIndexMap() {
    SetIndexMap(nullptr, IndexType::NONE);
}
void Mesh::SetIndexMap(std::shared_ptr<renderer::Buffer> iBuffer, IndexType iType) {
    assert(iBuffer != nullptr && iType != IndexType::NONE && "Index type must be none if iBuffer is nullptr");
    renderer::Mesh::SetIndexMap(iBuffer, iType);
    Bind();
    if (iBuffer != nullptr) {
        auto openglIBuffer = std::dynamic_pointer_cast<opengl::Buffer>(iBuffer);
        if (!openglIBuffer) {
            throw std::runtime_error("renderer/opengl/mesh.cpp: Failed to cast iBuffer to opengl::Buffer");
        }
        openglIBuffer->Bind();
    }
}

} // namespace opengl

} // namespace renderer

} // namespace marathon
