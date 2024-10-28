#include "renderer/buffer.hpp"
#include "renderer/opengl/opengl.hpp"

#include <unordered_map>

namespace marathon {

namespace renderer {

namespace opengl {

class Buffer : public renderer::Buffer {
protected:
    friend class Renderer;
    friend class Mesh;

    GLuint _id = 0;

    static const std::unordered_map<BufferTarget, GLenum> s_targetMap;
    static const std::unordered_map<BufferUsage, GLenum> s_usageMap;

    Buffer(void* data, size_t size, BufferTarget target, BufferUsage usage=BufferUsage::STATIC);

public:
    ~Buffer();

    /// --- Backend Impl ---
    void Bind() override;
    void Unbind() override;

    void SetData(void* data, size_t byteSize) override;
    void SetData(void* data, size_t byteSize, BufferUsage usage) override;
    void SetDataSubset(void* data, size_t size, size_t offset) override;

};

} // opengl

} // renderer

} // marathon
