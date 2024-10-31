#include "renderer/mesh.hpp"

namespace marathon {

namespace renderer {

const std::unordered_map<VertexAttributeFormat, size_t> Mesh::s_vertexAttributeFormatMap = {
    {VertexAttributeFormat::HALF_FLOAT, 2},
    {VertexAttributeFormat::FLOAT, 4},
    {VertexAttributeFormat::DOUBLE, 8},
    {VertexAttributeFormat::INT8, 1},
    {VertexAttributeFormat::INT16, 2},
    {VertexAttributeFormat::INT32, 4},
    {VertexAttributeFormat::INT64, 8},
    {VertexAttributeFormat::UINT8, 1},
    {VertexAttributeFormat::UINT16, 2},
    {VertexAttributeFormat::UINT32, 4},
    {VertexAttributeFormat::UINT64, 8}
};

const std::unordered_map<IndexFormat, size_t> Mesh::s_indexFormatMap = {
    {IndexFormat::UINT8, 1},
    {IndexFormat::UINT16, 2},
    {IndexFormat::UINT32, 4}
};

const std::unordered_map<VertexAttribute, int> Mesh::s_vertexAttributeLayoutMap = {
    {VertexAttribute::POSITION, 0},
    {VertexAttribute::NORMAL, 1},
    {VertexAttribute::TANGENT, 2},
    {VertexAttribute::COLOUR, 3},
    {VertexAttribute::TEXCOORD0, 4},
    {VertexAttribute::TEXCOORD1, 5},
    {VertexAttribute::TEXCOORD2, 6},
    {VertexAttribute::TEXCOORD3, 7},
    {VertexAttribute::TEXCOORD4, 8},
    {VertexAttribute::TEXCOORD5, 9},
    {VertexAttribute::TEXCOORD6, 10},
    {VertexAttribute::TEXCOORD7, 11}
};

bool Mesh::IsVertexBufferDirty(int vb) const { 
    if (vb < 0 || vb >= 4)
        return false;
    return _vertexStreamsDirty[vb];
}
void Mesh::SetVertexBufferClean(int vb) {
    if (vb < 0 || vb >= 4)
        return;
    _vertexStreamsDirty[vb] = false;
}
bool Mesh::IsIndexBufferDirty() const {
    return _isIndexStreamDirty;
}
void Mesh::SetIndexBufferClean() {
    _isIndexStreamDirty = false;
}

int Mesh::GetVertexAttributeIndex(VertexAttribute attr) const {
    auto it = _vertexAttributeIndexMap.find(attr);
    if (it != _vertexAttributeIndexMap.end())
        return it->second;
    return -1;
}

Mesh::~Mesh() {}

void Mesh::ClearVertex(bool keepLayout=true) {
    // _vertexCount = 0;
    // _vertexStreams.fill(nullptr);
    // _vertexStreamsDirty.fill(false);
    // _vertexAttributes.clear();
    // _vertexAttributeIndexMap.clear();
    // if (!keepLayout)
    //     _vertexAttributeIndexMap.clear();
}

// vertices
int Mesh::GetVertexAttributeCount() const {}
VertexAttributeDescriptor Mesh::GetVertexAttribute(int index) const {}
std::vector<VertexAttributeDescriptor> Mesh::GetVertexAttributes() const {}
VertexAttributeFormat Mesh::GetVertexAttributeFormat(VertexAttribute attr) const {}
int Mesh::GetVertexAttributeComponents(VertexAttribute attr) const {}
int Mesh::GetVertexAttributeStride(VertexAttribute attr) const {}
int Mesh::GetVertexAttributeOffset(VertexAttribute attr) const {}
int Mesh::GetVertexAttributeStream(VertexAttribute attr) const {}
int Mesh::GetVertexBufferStride(int stream) const {}
bool Mesh::HasVertexAttribute(VertexAttribute attr) const {}
void Mesh::SetVertexBufferParams(int vertexCount, std::vector<VertexAttributeDescriptor> attributes) {}
void Mesh::SetVertexBufferData(void* data, int src_start, int dest_start, int count, int stream) {}

// indices
int Mesh::GetIndexCount() const {}
IndexFormat Mesh::GetIndexFormat() const {}
PrimitiveType Mesh::GetPrimitiveType() const {}
void Mesh::SetIndexBufferParams(int indexCount, IndexFormat format) {}
void Mesh::SetIndexBufferData(void* data, int src_start, int dest_start, int count) {}

} // namespace renderer

} // namespace marathon