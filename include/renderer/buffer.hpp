#pragma once

// PRIVATE HEADER

#include <cstdlib>

#include "core/resource.hpp"

namespace marathon {

namespace renderer {

/// TODO:
// add buffer access control (read/copy/draw)

enum class BufferTarget {
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
    friend class Mesh;
    friend class Buffer;

    size_t _size;
    BufferTarget _target;
    BufferUsage _usage;

    Buffer(const std::string& name, void* data, size_t size, BufferTarget target, BufferUsage usage=BufferUsage::STATIC);
    virtual ~Buffer();

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

public:
    // set data by reallocating buffer
    virtual void SetData(void* data, size_t byteSize) = 0;
    virtual void SetData(void* data, size_t byteSize, BufferUsage usage) = 0;

    // faster than SetData, no reallocation
    virtual void SetDataSubset(void* data, size_t size, size_t offset) = 0;

    size_t GetSize() const;
    BufferTarget GetTarget() const;
    BufferUsage GetUsage() const;
};

} // renderer

} // marathon
