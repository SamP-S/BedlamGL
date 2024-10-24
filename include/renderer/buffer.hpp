#pragma once

// PRIVATE HEADER

#include <cstdlib>

#include "core/resource.hpp"

namespace marathon {

namespace renderer {

/*


Mesh:flush	Immediately sends all modified vertex data in the Mesh to the graphics card.		


Mesh:getDrawMode	Gets the mode used when drawing the Mesh.		
Mesh:getDrawRange	Gets the range of vertices used when drawing the Mesh.		
Mesh:getImage	Gets the Image used when drawing the Mesh.		
Mesh:getTexture	Gets the texture (Image or Canvas) used when drawing the Mesh.		
Mesh:getVertex	Gets the properties of a vertex in the Mesh.		
Mesh:getVertexAttribute	Gets the properties of a specific attribute within a vertex in the Mesh.		
Mesh:getVertexCount	Gets the total number of vertices in the Mesh.		
Mesh:getVertexFormat	Gets the vertex format that the Mesh was created with.		
Mesh:getVertexMap	Gets the vertex map for the Mesh.		
Mesh:getVertices	Gets all the vertices in the Mesh.		

Mesh:hasVertexColors	Gets whether per-vertex colors are used when drawing the Mesh.		
Mesh:isAttributeEnabled	Gets whether a specific vertex attribute in the Mesh is enabled.		

Mesh:setAttributeEnabled	Enables or disables a specific vertex attribute in the Mesh.		
Mesh:setDrawMode	Sets the mode used when drawing the Mesh.		
Mesh:setDrawRange	Restricts the drawn vertices of the Mesh to a subset of the total.		
Mesh:setImage	Sets the Image used when drawing the Mesh.		
Mesh:setTexture	Sets the texture (Image or Canvas) used when drawing the Mesh.		
Mesh:setVertex	Sets the properties of a vertex in the Mesh.		
Mesh:setVertexAttribute	Sets the properties of a specific attribute within a vertex in the Mesh.		
Mesh:setVertexColors	Sets whether per-vertex colors are used instead of the constant color when drawing the Mesh.		
Mesh:setVertexMap	Sets the vertex map for the Mesh.		
Mesh:setVertices	Replaces a range of vertices in the Mesh with new ones.	

*/

/// TODO:
// add buffer access control (read/copy/draw)

enum class BufferType {
    VERTEX,
    INDEX
    // texture
    // uniform
};


enum class BufferUsage {
    STATIC,
    DYNAMIC,
    STREAM
};

/// TODO:
// support copying data from another buffer
// consider keeping a copy of the data in our buffer object

// NOTE: Buffers can be resized after creation but its slow
//       it is better to create a larger buffer and modify subsets of it
//       Buffer type is fixed once created
//       If null given as data pointer, then buffer is created and filled with 0s 

class Buffer : public Resource {
protected:
    friend class Renderer;
    size_t _size;
    BufferType _type;
    BufferUsage _usage;

    Buffer(const std::string& name, void* data, size_t size, BufferType type, BufferUsage usage=BufferUsage::STATIC);
    virtual ~Buffer();

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

public:
    // set data by reallocating buffer
    virtual void SetData(void* data, size_t byteSize) = 0;
    virtual void SetData(void* data, size_t byteSize, BufferUsage usage) = 0;

    // faster than SetData, no reallocation
    virtual void SetDataSubset(void* data, size_t size, size_t offset) = 0;
    virtual void SetDataSubset(void* data, size_t size, size_t offset) = 0;

    size_t GetSize() const;
    BufferType GetType() const;
    BufferUsage GetUsage() const;
};

} // renderer

} // marathon
