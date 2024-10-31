#pragma once

// PUBLIC HEADER

#include <vector>
#include <string>
#include <utility>
#include <memory>
#include <cstdlib>
#include <unordered_map>
#include <array>
#include <stdexcept>    

#include "la_extended.h"
#include "core/resource.hpp"
#include "renderer/buffer.hpp"

namespace marathon {

namespace renderer {

enum class PrimitiveType {
    TRIANGLES,
    FAN,
    STRIP
};

enum class VertexAttributeFormat {
    // floating
    HALF_FLOAT, // 16-bit
    FLOAT,      // 32-bit
    DOUBLE,     // 64-bit
    // signed
    INT8, 
    INT16,
    INT32,
    INT64,
    // unsigned
    UINT8,
    UINT16,
    UINT32,
    UINT64
};

enum class IndexFormat {
    UINT8,
    UINT16,
    UINT32,
};

enum class VertexAttribute {
    POSITION,
    NORMAL,
    TANGENT,
    COLOUR,
    TEXCOORD0,
    TEXCOORD1,
    TEXCOORD2,
    TEXCOORD3,
    TEXCOORD4,
    TEXCOORD5,
    TEXCOORD6,
    TEXCOORD7
};

/// Vertex attribute descriptor for mesh vertex data layout
/// describes a single vertex attribute
struct VertexAttributeDescriptor {
    VertexAttribute attribute = VertexAttribute::POSITION;
    VertexAttributeFormat format = VertexAttributeFormat::FLOAT;
    int numComponents = 3;
    bool normalised = false;
    int stream = 0;
};

/// TODO: 
/// - add bound calculations for physics to use
/// - add instancing support
/// - add support for setting buffer usage (currently STATIC only)
/// - add validation on data being input, accepts absolute shit atm, throw errors for the factor to catch
/// - additionally add flags to allow for data validation checks to be skipped for performance
/// - improve performance by using ptr array fills for gets and passing const references
/// - add skeleton bones and stuff

/// CONSIDER: allow for usage to be set per stream rather than per mesh

class Mesh : public Resource {
protected:
    static const std::unordered_map<VertexAttributeFormat, size_t> s_vertexAttributeFormatMap;
    static const std::unordered_map<IndexFormat, size_t> s_indexFormatMap;
    static const std::unordered_map<VertexAttribute, int> s_vertexAttributeLayoutMap;

    // vertices
    int _vertexCount = 0;
    std::vector<VertexAttributeDescriptor> _vertexAttributes;
    std::unordered_map<VertexAttribute, int> _vertexAttributeIndexMap;
    std::array<void*, 4> _vertexStreams;
    std::array<bool, 4> _vertexStreamsDirty;
    
    // indices
    IndexFormat _indexFormat = IndexFormat::UINT32;
    int _indexCount = 0;
    PrimitiveType _primitive = PrimitiveType::TRIANGLES;
    void* _indexStream = nullptr;
    bool _isIndexStreamDirty = false;

    // check/manage dirty state
    bool IsVertexBufferDirty(int vb) const;
    void SetVertexBufferClean(int vb);
    bool IsIndexBufferDirty() const;
    void SetIndexBufferClean();

    // helper functions
    int GetVertexAttributeIndex(VertexAttribute attr) const;

public:
    // create mesh with vertex data
    ~Mesh();

    // clear/deletes all vertex data, index data, data sizes, but optionally keeps attrib layout
    void ClearVertex(bool keepLayout=true);

    // vertices
    int GetVertexAttributeCount() const;
    VertexAttributeDescriptor GetVertexAttribute(int index) const;
    std::vector<VertexAttributeDescriptor> GetVertexAttributes() const;
    VertexAttributeFormat GetVertexAttributeFormat(VertexAttribute attr) const;
    int GetVertexAttributeComponents(VertexAttribute attr) const;
    int GetVertexAttributeStride(VertexAttribute attr) const;
    int GetVertexAttributeOffset(VertexAttribute attr) const;
    int GetVertexAttributeStream(VertexAttribute attr) const;
    int GetVertexBufferStride(int stream) const;
    bool HasVertexAttribute(VertexAttribute attr) const;
    void SetVertexBufferParams(int vertexCount, std::vector<VertexAttributeDescriptor> attributes);
    void SetVertexBufferData(void* data, int src_start, int dest_start, int count, int stream);

    // indices
    int GetIndexCount() const;
    IndexFormat GetIndexFormat() const;
    PrimitiveType GetPrimitiveType() const;
    void SetIndexBufferParams(int indexCount, IndexFormat format);
    void SetIndexBufferData(void* data, int src_start, int dest_start, int count);
};

} // renderer

} // marathon

