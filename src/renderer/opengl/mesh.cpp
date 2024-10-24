#include "renderer/opengl/mesh.hpp"

#include <cassert>

namespace marathon {

namespace renderer {

namespace opengl {


/// TODO:
// support for a simple wireframe mode to convert to LINE versions of primitive types
const std::unordered_map<AttributeType, GLenum> Mesh::s_attrTypeMap = {
    { AttributeType::FLOAT, GL_FLOAT },
    { AttributeType::UINT8, GL_UNSIGNED_BYTE },
    { AttributeType::UINT16, GL_UNSIGNED_SHORT },
    { AttributeType::UINT32, GL_UNSIGNED_INT }
};

const std::unordered_map<PrimitiveType, GLenum> Mesh::s_primitiveMap = {
    { PrimitiveType::TRIANGLES, GL_TRIANGLES },
    { PrimitiveType::FAN, GL_TRIANGLE_FAN },
    { PrimitiveType::STRIP, GL_TRIANGLE_STRIP }
};

const std::unordered_map<IndexType, GLenum> Mesh::s_indexTypeMap = {
    { IndexType::UINT8, GL_UNSIGNED_BYTE },
    { IndexType::UINT16, GL_UNSIGNED_SHORT },
    { IndexType::UINT32, GL_UNSIGNED_INT }
};

Mesh::Mesh(int vCount, size_t vSize, std::shared_ptr<Buffer> vBuf, std::vector<VertexAttribute> vAttrs, 
    PrimitiveType primitive=PrimitiveType::TRIANGLES)
    : Mesh::Mesh(vCount, vSize, vBuf, vAttrs, nullptr, IndexType::NONE, primitive) {
    assert(vBuf != nullptr && "Vertex buffer must not be null");
    assert(vCount > 0 && "Vertex count must be greater than 0");
    assert(vAttrs.size() > 0 && "Vertex attributes must not be empty");
}

Mesh::Mesh(int vCount, size_t vSize, std::shared_ptr<Buffer> vBuf, std::vector<VertexAttribute> vAttrs, 
    std::shared_ptr<Buffer> iBuf, IndexType iType, PrimitiveType primitive=PrimitiveType::TRIANGLES)
    : renderer::Mesh("marathon.renderer.opengl.mesh", vCount, vSize, vBuf, vAttrs, iBuf, iType, primitive) {
    assert(vBuf != nullptr && "Vertex buffer must not be null");
    assert(vCount > 0 && "Vertex count must be greater than 0");
    assert(vAttrs.size() > 0 && "Vertex attributes must not be empty");
    assert(iBuf != nullptr && iType != IndexType::NONE && "Index type must not be NONE if index buffer is set");

    glGenVertexArrays(1, &_vao);
    Bind();
    _vBuffer->Bind();
    for (const auto& attr : _vAttrs) {
        glEnableVertexAttribArray(attr.index);
        glVertexAttribPointer(attr.index, attr.numComponents, s_attrTypeMap.at(attr.type),
            attr.normalised, attr.stride, (void*)attr.offset);
    }
    if (_iBuffer != nullptr) {
        _iBuffer->Bind();
    }
    Unbind();

}
Mesh::~Mesh() = default;

void Mesh::Bind() {
    assert(_vao != 0 && "VAO must be created before binding");
    glBindVertexArray(_vao);
}
void Mesh::Unbind() {
    glBindVertexArray(0);
}
void Mesh::Draw(Renderer& renderer, const LA::mat4& m) {
    assert(_vao != 0 && "VAO must be created before drawing");
}

void Mesh::SetIndexMap() {
    SetIndexMap(nullptr, IndexType::NONE);
}
void Mesh::SetIndexMap(std::shared_ptr<Buffer> iBuffer, IndexType iType=IndexType::UINT32) {
    assert(iBuffer != nullptr && iType != IndexType::NONE && "Index type must be none if iBuffer is nullptr");
    renderer::Mesh::SetIndexMap(iBuffer, iType);
    Bind();
    if (iBuffer != nullptr) {
        iBuffer->Bind();
    }
}

} // namespace opengl

} // namespace renderer

} // namespace marathon
