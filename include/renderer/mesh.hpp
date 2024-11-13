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
#include <cstring>

#include "la_extended.h"
#include "core/resource.hpp"

namespace marathon {

namespace renderer {

enum class PrimitiveType {
    INVALID,
    TRIANGLES,
    FAN,
    STRIP
};

enum class BufferFormat {
    INVALID,
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

enum class VertexAttributeFormat {
    // error
    INVALID,
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
    INVALID,
    UINT8,
    UINT16,
    UINT32
};

enum class VertexAttribute {
    INVALID,
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

enum class DataDirty {
    INVALID,
    DIRTY_REALLOC,
    DIRTY_UPDATE,
    DIRTY_DELETE,
    CLEAN
};

/// Vertex attribute descriptor for mesh vertex data layout
/// describes a single vertex attribute
struct VertexAttributeDescriptor {
    VertexAttribute attribute = VertexAttribute::POSITION;
    VertexAttributeFormat format = VertexAttributeFormat::FLOAT;
    int numComponents = 3;
};

/// TODO:
/// allow for multiple vertex buffers/data streams rather than a single interleaved vbo
/// allow for buffer usage to be set (currently STATIC only)
/// should centralise all raw data handling into a single buffer class
/// replace void* with c++ standard alternatives e.g. std::array for runtime allocated fixed array or std::vector for dynamicly sized array

/// TODO: 
/// - add bound calculations for physics to use
/// - add instancing support
/// - implement support for multiple vbos
/// - add support for setting buffer usage (currently STATIC only)
/// - add validation on data being input, accepts absolute shit atm, throw errors for the factor to catch
/// - additionally add flags to allow for data validation checks to be skipped for performance
/// - improve performance by using ptr array fills for gets and passing const references
/// - add skeleton bones and stuff
/// - add support for optional data normalisation, currently user must normalise data if required

/// CONSIDER: allow for usage to be set per stream rather than per mesh

class Mesh : public Resource {
protected:
    /// --- VBO ---
    // buffer
    void* _vertexData = nullptr; 
    DataDirty _vertexDataDirty = DataDirty::CLEAN;
    // vertex params
    int _vertexCount = 0;
    std::vector<VertexAttributeDescriptor> _vertexAttributeDescriptors = {};
    // vertex maps
    static const std::unordered_map<VertexAttributeFormat, size_t> s_vertexAttributeFormatMap;
    static const std::unordered_map<VertexAttribute, int> s_vertexAttributeLayoutMap;

    /// --- IBO ---
    // buffer
    void* _indexData = nullptr;
    DataDirty _indexDataDirty = DataDirty::CLEAN;
    // index params
    int _indexCount = 0;
    IndexFormat _indexFormat = IndexFormat::UINT16;
    PrimitiveType _primitive = PrimitiveType::TRIANGLES;
    // index maps
    static const std::unordered_map<IndexFormat, size_t> s_indexFormatMap;

    /// --- INTERNAL ---
    int GetVertexAttributeIndex(VertexAttribute attr) const;

public:
    // create empty mesh
    Mesh();
    ~Mesh();

    // clear all data
    void Clear();

    /// --- VBO ---
    // release memory and reset params
    void ClearVertices();
    // call to stop data being uploaded to GPU next frame
    void ClearVertexDirtyFlag();

    // getters
    const void* GetVertexPtr() const;
    int GetVertexCount() const;
    std::vector<VertexAttributeDescriptor> GetVertexAttributes() const;
    DataDirty GetVertexDirtyFlag() const;

    // helper getters
    bool HasVertexAttribute(VertexAttribute attr) const;
    int GetVertexAttributeLocation(VertexAttribute attr) const;
    int GetVertexAttributeComponents(VertexAttribute attr) const;
    VertexAttributeFormat GetVertexAttributeFormat(VertexAttribute attr) const;
    size_t GetVertexAttributeOffset(VertexAttribute attr) const;
    size_t GetVertexSize() const;
    
    // allocates memory according to attributes, vertex data expected to be interleaved in provided order
    void SetVertexParams(int vertexCount, std::vector<VertexAttributeDescriptor> attributes);
    // will error if size/offset data range outside expected
    void SetVertexData(void* data, size_t size, size_t src_start, size_t dest_start);


    /// --- IBO ---
    // release memory and reset params
    void ClearIndices();
    // call to stop data being uploaded to GPU next frame
    void ClearIndexDirtyFlag();

    // getters
    const void* GetIndexPtr() const;
    int GetIndexCount() const;
    size_t GetIndexSize() const;
    IndexFormat GetIndexFormat() const;
    PrimitiveType GetPrimitiveType() const;
    DataDirty GetIndexDirtyFlag() const;

    // set index data formatting
    void SetIndexParams(int indexCount, IndexFormat format, PrimitiveType primitive);
    // will error if size/offset data range outside expected
    // use SetIndexParams to reallocate buffer size if needed
    void SetIndexData(void* data, size_t size, size_t src_start, size_t dest_start);
};

} // renderer

} // marathon

