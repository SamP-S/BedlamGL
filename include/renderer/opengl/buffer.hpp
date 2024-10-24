#include "renderer/buffer.hpp"
#include "renderer/opengl/opengl.hpp"

#include <unordered_map>

namespace marathon {

namespace renderer {

namespace opengl {

class Buffer : public renderer::Buffer {
protected:
    friend class Renderer;
    GLuint _id = 0;

    std::unordered_map<BufferTarget, GLenum> _targetMap = {
        {BufferTarget::VERTEX, GL_ARRAY_BUFFER},
        {BufferTarget::INDEX, GL_ELEMENT_ARRAY_BUFFER}
    };

    std::unordered_map<BufferUsage, GLenum> _usageMap = {
        {BufferUsage::STATIC, GL_STATIC_DRAW},
        {BufferUsage::DYNAMIC, GL_DYNAMIC_DRAW},
        {BufferUsage::STREAM, GL_STREAM_DRAW}
    };


    Buffer(void* data, size_t size, BufferTarget target, BufferUsage usage=BufferUsage::STATIC);
    ~Buffer();

    /// --- Backend Impl ---
    void Bind() override;
    void Unbind() override;

public:
    void SetData(void* data, size_t byteSize) override;
    void SetData(void* data, size_t byteSize, BufferUsage usage) override;
    void SetDataSubset(void* data, size_t size, size_t offset) override;

};

} // opengl

} // renderer

} // marathon
