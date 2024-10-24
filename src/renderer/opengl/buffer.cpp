#include "renderer/opengl/buffer.hpp"

namespace marathon {

namespace renderer {

namespace opengl {

Buffer::Buffer(void* data, size_t size, BufferTarget target, BufferUsage usage)
    : Buffer(data, size, target, usage) {
    glGenBuffers(1, &_id);
    Bind();
    glBufferData(_targetMap[_target], size, data, _usageMap[_usage]);
    Unbind();
}
Buffer::~Buffer() {
    glDeleteBuffers(1, &_id);
}

void Buffer::Bind() {
    glBindBuffer(_targetMap[_target], _id);
}
void Buffer::Unbind() {
    glBindBuffer(_targetMap[_target], 0);
}

void Buffer::SetData(void* data, size_t byteSize) {
    Bind();
    glBufferData(_targetMap[_target], byteSize, data, _usageMap[_usage]);
    Unbind();
}
void Buffer::SetData(void* data, size_t byteSize, BufferUsage usage) {
    _usage = usage;
    SetData(data, byteSize)
}
void Buffer::SetDataSubset(void* data, size_t size, size_t offset) {
    Bind();
    glBufferSubData(_targetMap[_target], offset, size, data);
    Unbind();
}


} // opengl

} // renderer

} // marathon
