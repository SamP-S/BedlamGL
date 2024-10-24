#include "renderer/buffer.hpp"

namespace marathon {

namespace renderer {

Buffer::Buffer(const std::string& name, void* data, size_t size, BufferType type, BufferUsage usage=BufferUsage::STATIC)
    : Resource(name), _size(size), _type(type), _usage(usage) {}

Buffer::~Buffer() {}

size_t Buffer::GetSize() const { return _size; }
BufferType Buffer::GetType() const { return _type; }
BufferUsage Buffer::GetUsage() const { return _usage; }
    

} // namespace renderer

} // namespace marathon