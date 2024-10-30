#pragma once

// PUBLIC HEADER

#include <vector>
#include <string>
#include <utility>
#include <memory>
#include <cstdlib>
#include <unordered_map>

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
    // unsigned
    UINT8,
    UINT16,
    UINT32
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
    BITANGENT,
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

struct VertexAttributeDescriptor {
    int index;
    int numComponents;
    VertexAttributeFormat format;
    int stride = 0;
    int offset = 0;
    bool normalised = false;
    int stream = 0;
};

/// TODO: 
/// - add bound calculations for physics to use
/// - add instancing support
/// - add support for setting buffer usage (currently STATIC only)

/// CONSIDER: allow for usage to be set per stream rather than per mesh

class Mesh : public Resource {
protected:
    // vertices
    int _vertexCount = 0;
    int _vertexAttributeCount = 0;
    std::vector<VertexAttributeDescriptor> _vertexAttributes;
    
    // indices
    IndexFormat _indexFormat = IndexFormat::UINT32;
    int _indexCount = 0;
    PrimitiveType _primitive = PrimitiveType::TRIANGLES;

public:
    // create mesh with vertex data
    ~Mesh();

    // clear all vertex and index data
    // clear counts but optionally keep layout
    // effectively a big delete
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
    int GetVertexStreamStride(int stream) const;
    bool HasVertexAttribute(VertexAttribute attr) const;
    /// TODO:
    // Add validation on data being input, accepts absolute shit atm, throw errors for the factor to catch
    void SetVertexBufferParams(int vertexCount, std::vector<VertexAttributeDescriptor> attributes);
    void SetVertexBufferData(void* data, int src_start, int dest_start, int count, int stream);

    // indices
    int GetIndexCount() const;
    IndexFormat GetIndexFormat() const;
    PrimitiveType GetPrimitiveType() const;
    /// CONSIDER: adding flags for disabling verification
    /// TODO:
    // Add validation on data being input, accepts absolute shit atm, throw errors for the factor to catch
    void SetIndexBufferParams(int indexCount, IndexFormat format);
    void SetIndexBufferData(void* data, int src_start, int dest_start, int count);
};

} // renderer

} // marathon

