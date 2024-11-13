#include "renderer/buffer.hpp"

namespace marathon {

namespace renderer {

Buffer::Buffer(const std::string& name, void* data, size_t size, BufferTarget target, BufferUsage usage)
    : Resource(name), _size(size), _target(target), _usage(usage) {}

Buffer::~Buffer() {}

size_t Buffer::GetSize() const { return _size; }
BufferTarget Buffer::GetTarget() const { return _target; }
BufferUsage Buffer::GetUsage() const { return _usage; }
    

} // namespace renderer

} // namespace marathon