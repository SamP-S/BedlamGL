#pragma once

// PUBLIC HEADER

#include <vector>
#include <string>
#include <utility>
#include <memory>
#include <cstdlib>
#include <unordered_map>

#include "renderer/buffer.hpp"
#include "renderer/drawable.hpp"

namespace marathon {

namespace renderer {

// NOTE: Default assets should be baked into code, as removes need for pathing and loading from external files
/// TODO: Move these or hide them inside Renderer module
static const std::vector<LA::vec3> defaultTriangleVertices = {
    {  0.0f,  1.0f, 0.0f },
    { -1.0f, -1.0f, 0.0f },
    {  1.0f, -1.0f, 0.0f }
};

static const std::vector<LA::vec3> defaultQuadVertices = {
    { -1.0f, -1.0f, 0.0f },
    {  1.0f, -1.0f, 0.0f },
    {  1.0f,  1.0f, 0.0f },
    { -1.0f,  1.0f, 0.0f }
};
static const std::vector<uint32_t> defaultQuadIndices = {
    0, 1, 2, 2, 3, 0
};

static const std::vector<LA::vec3> defaultCubeVertices = {
    // front
    { -1.0f, -1.0f,  1.0f },
    {  1.0f, -1.0f,  1.0f },
    {  1.0f,  1.0f,  1.0f },
    { -1.0f,  1.0f,  1.0f },
    // back
    { -1.0f, -1.0f, -1.0f },
    {  1.0f, -1.0f, -1.0f },
    {  1.0f,  1.0f, -1.0f },
    { -1.0f,  1.0f, -1.0f }
};
static const std::vector<uint32_t> defaultCubeIndices = {
    0, 1, 2, 2, 3, 0,
    1, 5, 6, 6, 2, 1,
    7, 6, 5, 5, 4, 7,
    4, 0, 3, 3, 7, 4,
    3, 2, 6, 6, 7, 3,
    4, 5, 1, 1, 0, 4
};


enum class PrimitiveType {
    TRIANGLES,
    FAN,
    STRIP
};

enum class AttributeType {
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

enum class IndexType {
    UINT8,
    UINT16,
    UINT32,
    NONE
};

struct VertexAttribute {
    int index;
    int numComponents;
    AttributeType type;
    int stride = 0;
    int offset = 0;
    bool normalised = false;
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
class Mesh : public Drawable {
protected:
    friend class Renderer;
    
    int _vertexCount = 0;
    size_t _vertexSize = 0;
    PrimitiveType _primitive = PrimitiveType::TRIANGLES;


    std::shared_ptr<Buffer> _vBuffer = nullptr;
    std::vector<VertexAttribute> _vAttrs = {};
    std::shared_ptr<Buffer> _iBuffer = nullptr;
    IndexType _indexType = IndexType::NONE;

    // create mesh with vertex data
    Mesh(const std::string& name, int vCount, size_t vSize, std::shared_ptr<Buffer> vBuf, std::vector<VertexAttribute> vAttrs, 
        PrimitiveType primitive=PrimitiveType::TRIANGLES);
    // create mesh with vertex data and index map
    Mesh(const std::string& name, int vCount, size_t vSize, std::shared_ptr<Buffer> vBuf, std::vector<VertexAttribute> vAttrs, 
        std::shared_ptr<Buffer> iBuf, IndexType iType, PrimitiveType primitive=PrimitiveType::TRIANGLES);
    

public:
    virtual ~Mesh();

    /// --- Backend Impl ---
    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    // drawable
    virtual void Draw(Renderer& renderer) = 0;


    /// --- Properties ---
    PrimitiveType GetPrimitiveType() const;
    void SetPrimitiveType(PrimitiveType primitive);


    /// --- Index Data ---
    // unsets index buffer
    virtual void SetIndexMap();
    // set index buffer
    virtual void SetIndexMap(std::shared_ptr<Buffer> iBuffer, IndexType iType=IndexType::UINT32);
    // get index buffer
    // returns true if index buffer is set and fills args
    // returns false if no index buffer set, args are set nullptr
    // bool GetIndexMap(Buffer* buf, IndexType* type) const;
    
};

} // renderer

} // marathon

