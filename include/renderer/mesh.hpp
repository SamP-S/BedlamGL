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
    NONE,
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
    NONE,
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
// add support for setting a draw range (glDraw from start using count)
// allow enable/disable attributes (glEnableVertexAttribArray/glDisableVertexAttribArray)
// support a default data layout for default shaders
// support changing vertex count after creation
// fix access issues to prevent changing buffer data without updating vertex attributes
// allow read access to vertex attributes
// allow read access to index buffer

// CONSIDER: allowing vertex format class and support changing vertex attributes after creation

// HARD: add support for instanced rendering
// HARD: add support for multiple vertex buffers for different vertex attributes
//       probably requires the user to create buffers themsevles through the Renderer

// NOTE: mesh vertex count is fixed once created
//       mesh index map data can be resized/changed
class Mesh : public Resource {
protected:
    // vertices
    int _vertexCount = 0;
    int _vertexAttributeCount = 0;
    std::vector<VertexAttributeDescriptor> _vertexAttributes;
    
    // indices
    IndexFormat _indexFormat = IndexFormat::NONE;
    int _indexCount = 0;
    PrimitiveType _primitive = PrimitiveType::NONE;

public:
    // create mesh with vertex data
    Mesh(const std::string& name);
    ~Mesh();

    // vertices
    int GetVertexAttributeCount() const;
    VertexAttributeDescriptor GetVertexAttribute(int index) const;
    std::vector<VertexAttributeDescriptor> GetVertexAttributes() const;
    int GetVertexAttributeComponents(VertexAttribute attr) const;
    VertexAttributeFormat GetVertexAttributeFormat(VertexAttribute attr) const;
    int GetVertexAttributeStride(VertexAttribute attr) const;
    int GetVertexAttributeOffset(VertexAttribute attr) const;
    int GetVertexAttributeStream(VertexAttribute attr) const;
    
    void SetVertexBufferParams(int vertexCount, std::vector<VertexAttributeDescriptor> attributes);
    void SetVertexBufferData(void* data, int src_start, int dest_start, int count, int stream);


    // indices
    IndexFormat GetIndexFormat() const;
    int GetIndexCount() const;
    PrimitiveType GetPrimitiveType() const;
    /// CONSIDER: adding flags for disabling verification
    void SetIndexBufferParams(int indexCount, IndexFormat format);
    void SetIndexBufferData(void* data, int src_start, int dest_start, int count);
};

} // renderer

} // marathon

