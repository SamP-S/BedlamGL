#include "renderer/mesh.hpp"

namespace marathon {

namespace renderer {


/// --- INTERNAL ---
/// TODO: Implement a dictionary to cache indexes instead of linear search everytime for big speed improvement
// find index of attribute in descriptor list or return -1 if not found
int Mesh::GetVertexAttributeIndex(VertexAttribute attr) const {
    for (int i = 0; i < _vertexAttributeDescriptors.size(); i++) {
        if (_vertexAttributeDescriptors[i].attribute == attr) {
            return i;
        }
    }
    return -1;
}



/// --- VBO ---
const std::unordered_map<VertexAttributeFormat, size_t> Mesh::s_vertexAttributeFormatMap = {
    {VertexAttributeFormat::INVALID, 0},
    {VertexAttributeFormat::HALF_FLOAT, 2},
    {VertexAttributeFormat::FLOAT, 4},
    {VertexAttributeFormat::DOUBLE, 8},
    {VertexAttributeFormat::INT8, 1},
    {VertexAttributeFormat::INT16, 2},
    {VertexAttributeFormat::INT32, 4},
    {VertexAttributeFormat::UINT8, 1},
    {VertexAttributeFormat::UINT16, 2},
    {VertexAttributeFormat::UINT32, 4}
};

const std::unordered_map<VertexAttribute, int> Mesh::s_vertexAttributeLayoutMap = {
    {VertexAttribute::INVALID, -1},
    {VertexAttribute::POSITION, 0},
    {VertexAttribute::NORMAL, 1},
    {VertexAttribute::TANGENT, 2},
    {VertexAttribute::COLOUR, 3},
    {VertexAttribute::TEXCOORD0, 4},
    {VertexAttribute::TEXCOORD1, 5},
    {VertexAttribute::TEXCOORD2, 6},
    {VertexAttribute::TEXCOORD3, 7}
};

void Mesh::ClearVertices() {
    // Empty implementation
    /// TODO: Implement
    std::cout << "src/renderer/mesh.cpp: Mesh::ClearVertices() not implemented" << std::endl;
}

void Mesh::ClearVertexDirtyFlag() {
    _vertexDataDirty = DataDirty::CLEAN;
}

const void* Mesh::GetVertexPtr() const {
    return _vertexData;
}

int Mesh::GetVertexCount() const {
    return _vertexCount;
}

std::vector<VertexAttributeDescriptor> Mesh::GetVertexAttributes() const {
    return _vertexAttributeDescriptors;
}

DataDirty Mesh::GetVertexDirtyFlag() const {
    return _vertexDataDirty;
}

// return false if failed to find attribute
bool Mesh::HasVertexAttribute(VertexAttribute attr) const {
    return GetVertexAttributeIndex(attr) != -1;
}

// returns -1 if failed
int Mesh::GetVertexAttributeLocation(VertexAttribute attr) const {
    auto it = s_vertexAttributeLayoutMap.find(attr);
    if (it != s_vertexAttributeLayoutMap.end()) {
        return it->second;
    } else {
        return -1;
    }
}

// return 0 if failed
int Mesh::GetVertexAttributeComponents(VertexAttribute attr) const {
    int idx = GetVertexAttributeIndex(attr);
    if (idx == -1) 
        return 0;
    else
        return _vertexAttributeDescriptors.at(idx).numComponents;
}

// return VertexAttributeFormat::INVALID if failed
VertexAttributeFormat Mesh::GetVertexAttributeFormat(VertexAttribute attr) const {
    int idx = GetVertexAttributeIndex(attr);
    if (idx == -1) 
        return VertexAttributeFormat::INVALID;
    else
        return _vertexAttributeDescriptors.at(idx).format;
}

// return 0 if failed
size_t Mesh::GetVertexAttributeOffset(VertexAttribute attr) const {
    int idx = GetVertexAttributeIndex(attr);
    if (idx == -1) {
        return 0;
    }

    size_t offset = 0;
    for (int i = 0; i < idx; i++) {
        offset += s_vertexAttributeFormatMap.at(_vertexAttributeDescriptors[i].format) * _vertexAttributeDescriptors[i].numComponents;
    }
    return offset;
}

size_t Mesh::GetVertexSize() const {
    size_t size = 0;
    for (int i = 0; i < _vertexAttributeDescriptors.size(); i++) {
        size += s_vertexAttributeFormatMap.at(_vertexAttributeDescriptors[i].format) * _vertexAttributeDescriptors[i].numComponents;
    }
    return size;
}

void Mesh::SetVertexParams(int vertexCount, std::vector<VertexAttributeDescriptor> attributes) {
    ClearVertices();
    _vertexCount = vertexCount;
    _vertexAttributeDescriptors = attributes;
    _vertexData = malloc(GetVertexSize() * vertexCount);
    _vertexDataDirty = DataDirty::DIRTY_REALLOC;
}

void Mesh::SetVertexData(void* data, size_t size, size_t src_start, size_t dest_start) {
    size_t vertexSize = GetVertexSize();
    size_t dataSize = vertexSize * _vertexCount;
    // catch fucky wuckys
    if (data == nullptr) {
        std::cout << "src/renderer/mesh.cpp: WARNING @ Mesh::SetVertexData() data is nullptr" << std::endl;
        return;
    } else if (_vertexData == nullptr) {
        std::cout << "src/renderer/mesh.cpp: WARNING @ Mesh::SetVertexData() vertex data is nullptr" << std::endl;
        return;
    } else if (size + src_start > dataSize - dest_start) {
        std::cout << "src/renderer/mesh.cpp: WARNING @ Mesh::SetVertexData() data range out of bounds" << std::endl;
        return;
    }
    // copy data
    memcpy(_vertexData + dest_start, data + src_start, size);
    // realloc takes precident over update
    if (_vertexDataDirty != DataDirty::DIRTY_REALLOC) 
        _vertexDataDirty = DataDirty::DIRTY_UPDATE;
}



/// --- IBO --- ///
const std::unordered_map<IndexFormat, size_t> Mesh::s_indexFormatMap = {
    {IndexFormat::INVALID, 0},
    {IndexFormat::UINT8, 1},
    {IndexFormat::UINT16, 2},
    {IndexFormat::UINT32, 4}
};

void Mesh::ClearIndices() {
    /// TODO: implement
    std::cout << "src/renderer/mesh.cpp: Mesh::ClearIndices() not implemented" << std::endl;
}

void Mesh::ClearIndexDirtyFlag() {
    _indexDataDirty = DataDirty::CLEAN;
}

const void* Mesh::GetIndexPtr() const {
    return _indexData;
}

int Mesh::GetIndexCount() const {
    return _indexCount;
}

size_t Mesh::GetIndexSize() const {
    return s_indexFormatMap.at(_indexFormat);
}

DataDirty Mesh::GetIndexDirtyFlag() const {
    return _indexDataDirty;
}

IndexFormat Mesh::GetIndexFormat() const {
    return _indexFormat;
}

PrimitiveType Mesh::GetPrimitiveType() const {
    return _primitive;
}

void Mesh::SetIndexParams(int indexCount, IndexFormat format, PrimitiveType primitive) {
    ClearIndices();
    _indexCount = indexCount;
    _indexFormat = format;
    _primitive = primitive;
    _indexData = malloc(s_indexFormatMap.at(format) * indexCount);
    _indexDataDirty = DataDirty::DIRTY_REALLOC;
}

void Mesh::SetIndexData(void* data, size_t size, size_t src_start, size_t dest_start) {
    size_t dataSize = s_indexFormatMap.at(_indexFormat) * _indexCount;
    // catch bad args
    if (data == nullptr) {
        std::cout << "src/renderer/mesh.cpp: WARNING @ Mesh::SetIndexData() data is nullptr" << std::endl;
        return;
    } else if (_indexData == nullptr) {
        std::cout << "src/renderer/mesh.cpp: WARNING @ Mesh::SetIndexData() index data is nullptr" << std::endl;
        return;
    } else if (size + src_start > dataSize - dest_start) {
        std::cout << "src/renderer/mesh.cpp: WARNING @ Mesh::SetIndexData() data range out of bounds" << std::endl;
        return;
    }
    memcpy(_indexData + dest_start, data + src_start, size);
    // realloc takes precident over update
    if (_indexDataDirty != DataDirty::DIRTY_REALLOC) 
        _indexDataDirty = DataDirty::DIRTY_UPDATE;

}



/// --- MESH --- ///

Mesh::Mesh() 
    : Resource("marathon.renderer.mesh") {}
Mesh::~Mesh() {}

void Mesh::Clear() {
    ClearVertices();
    ClearIndices();
    _indexDataDirty = DataDirty::DIRTY_DELETE;
}


} // namespace renderer

} // namespace marathon