#pragma once

#include <unordered_map>

#include "renderer/mesh.hpp"
#include "renderer/opengl/opengl.hpp"


namespace marathon {

namespace renderer {

namespace opengl {

class Mesh : public renderer::Mesh {
protected:
    friend class Renderer;
    friend class renderer::Renderer;

    GLuint _vao = 0;

    static const std::unordered_map<AttributeType, GLenum> s_attrTypeMap;
    static const std::unordered_map<PrimitiveType, GLenum> s_primitiveMap;
    static const std::unordered_map<IndexType, GLenum> s_indexTypeMap;

    Mesh(int vCount, size_t vSize, std::shared_ptr<renderer::Buffer> vBuf, std::vector<VertexAttribute> vAttrs, PrimitiveType primitive=PrimitiveType::TRIANGLES);
    Mesh(int vCount, size_t vSize, std::shared_ptr<renderer::Buffer> vBuf, std::vector<VertexAttribute> vAttrs, std::shared_ptr<renderer::Buffer> iBuf, IndexType iType, PrimitiveType primitive=PrimitiveType::TRIANGLES);


public:
    ~Mesh();

    void Bind() override;
    void Unbind() override;
    void Draw(renderer::Renderer& renderer) override;
    
    void SetIndexMap() override;
    void SetIndexMap(std::shared_ptr<renderer::Buffer> iBuffer, IndexType iType) override;

};

} // namespace opengl

} // namespace renderer

} // namespace marathon