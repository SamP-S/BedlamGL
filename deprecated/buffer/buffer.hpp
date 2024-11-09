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
    // should be removed
    friend class Renderer;

    size_t _size = 0;
    BufferTarget _target = BufferTarget::VERTEX;
    BufferUsage _usage = BufferUsage::STATIC;
    bool _isDirty = true;

public:
    // create empty buffer
    Buffer();
    virtual ~Buffer();

    /// TODO: bind/unbind should not be public
    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    // set buffer params and allocate
    virtual void SetParams(size_t size, BufferUsage usage=BufferUsage::STATIC, BufferTarget target=BufferTarget::VERTEX) = 0;

    // set data and reallocate buffer
    virtual void SetData(void* data, size_t size, BufferUsage usage=BufferUsage::STATIC) = 0;

    // faster than SetData, no reallocation
    virtual void SetDataFast(void* data, size_t size, size_t src_start, size_t dest_start) = 0;

    size_t GetSize() const;
    BufferTarget GetTarget() const;
    BufferUsage GetUsage() const;
};

} // renderer

} // marathon
