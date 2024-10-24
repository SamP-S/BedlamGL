#include "renderer/opengl/buffer.hpp"

namespace marathon {

namespace renderer {

namespace opengl {

const std::unordered_map<BufferTarget, GLenum> Buffer::s_targetMap = {
    {BufferTarget::VERTEX, GL_ARRAY_BUFFER},
    {BufferTarget::INDEX, GL_ELEMENT_ARRAY_BUFFER}
};

const std::unordered_map<BufferUsage, GLenum> Buffer::s_usageMap = {
    {BufferUsage::STATIC, GL_STATIC_DRAW},
    {BufferUsage::DYNAMIC, GL_DYNAMIC_DRAW},
    {BufferUsage::STREAM, GL_STREAM_DRAW}
};

Buffer::Buffer(void* data, size_t size, BufferTarget target, BufferUsage usage)
    : Buffer(data, size, target, usage) {
    glGenBuffers(1, &_id);
    Bind();
    glBufferData(s_targetMap.at(_target), size, data, s_usageMap.at(_usage));
    Unbind();
}
Buffer::~Buffer() {
    glDeleteBuffers(1, &_id);
}

void Buffer::Bind() {
    glBindBuffer(s_targetMap.at(_target), _id);
}
void Buffer::Unbind() {
    glBindBuffer(s_targetMap.at(_target), 0);
}

void Buffer::SetData(void* data, size_t byteSize) {
    Bind();
    glBufferData(s_targetMap.at(_target), byteSize, data, s_usageMap.at(_usage));
    Unbind();
}
void Buffer::SetData(void* data, size_t byteSize, BufferUsage usage) {
    _usage = usage;
    SetData(data, byteSize)
}
void Buffer::SetDataSubset(void* data, size_t size, size_t offset) {
    Bind();
    glBufferSubData(s_targetMap.at(_target), offset, size, data);
    Unbind();
}


} // opengl

} // renderer

} // marathon
