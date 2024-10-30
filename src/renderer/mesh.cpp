#include "renderer/mesh.hpp"

namespace marathon {

namespace renderer {

/// TODO:
// Add validation on data being input, accepts absolute shit atm, throw errors for the factor to catch

// constructor/destructor
Mesh::Mesh(const std::string& name, int vCount, size_t vSize, std::shared_ptr<Buffer> vBuf, std::vector<VertexAttribute> vAttrs, PrimitiveType primitive)
    : Resource(name),
    _vertexCount(vCount), _vertexSize(vSize), _primitive(primitive),
    _vBuffer(vBuf), _vAttrs(vAttrs) {}
Mesh::Mesh(const std::string& name, int vCount, size_t vSize, std::shared_ptr<Buffer> vBuf, std::vector<VertexAttribute> vAttrs, std::shared_ptr<Buffer> iBuf, IndexType iType, PrimitiveType primitive)
    : Resource(name),
    _vertexCount(vCount), _vertexSize(vSize), _primitive(primitive),
    _vBuffer(vBuf), _vAttrs(vAttrs),
    _iBuffer(iBuf), _indexType(iType) {}

Mesh::~Mesh() {}

// properties
PrimitiveType Mesh::GetPrimitiveType() const { return _primitive; }
void Mesh::SetPrimitiveType(PrimitiveType primitive) { _primitive = primitive; }

void Mesh::SetIndexMap() {
    _iBuffer = nullptr;
    _indexType = IndexType::NONE;
}
void Mesh::SetIndexMap(std::shared_ptr<Buffer> iBuffer, IndexType iType) {
    _iBuffer = iBuffer;
    _indexType = iType;
}

} // namespace renderer

} // namespace marathon