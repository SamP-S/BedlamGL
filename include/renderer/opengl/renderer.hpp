#pragma once

#include <unordered_map>


#include "renderer/opengl/opengl.hpp"
#include "renderer/renderer.hpp"

namespace marathon {

namespace renderer {

namespace opengl {

class Renderer : public renderer::Renderer {
protected:

    std::unordered_map<CullFace, GLenum> _cullFaceMap = {
        {CullFace::FRONT, GL_FRONT},
        {CullFace::BACK, GL_BACK},
        {CullFace::FRONT_AND_BACK, GL_FRONT_AND_BACK}
    };

    std::unordered_map<CullWinding, GLenum> _cullWindingMap = {
        {CullWinding::CLOCKWISE, GL_CW},
        {CullWinding::COUNTER_CLOCKWISE, GL_CCW}
    };

    std::unordered_map<DepthFunc, GLenum> _depthFuncMap = {
        {DepthFunc::NEVER, GL_NEVER},
        {DepthFunc::ALWAYS, GL_ALWAYS},
        {DepthFunc::EQUAL, GL_EQUAL},
        {DepthFunc::NOT_EQUAL, GL_NOTEQUAL},
        {DepthFunc::LESS, GL_LESS},
        {DepthFunc::LESS_EQUAL, GL_LEQUAL},
        {DepthFunc::GREATER, GL_GREATER},
        {DepthFunc::GREATER_EQUAL, GL_GEQUAL}
    };

    LA::mat4 _projection = LA::mat4();
    LA::mat4 _view = LA::mat4();
    std::stack<LA::mat4> _transforms;

public:
    Renderer();
    ~Renderer();

    // module interface
    void Boot() override;
    void Shutdown() override;

    /// --- Factories ---
    std::shared_ptr<Buffer> CreateBuffer() override;
    std::shared_ptr<Mesh> CreateMesh() override;
    std::shared_ptr<Shader> CreateShader() override;
    
    // validation
    bool ValidateShaderCode(const std::string code, ShaderType stageType, std::string& err) override;
    
    /// --- Drawing ---
    // clear active canvas/screen of all color, depth, and stencil buffers
    void Clear() override;
    // clear active canvas/screen according to bools args
    void Clear(bool clearColor, bool clearStencil, bool clearDepth) override;

    /// --- State Management ---
    void SetState(RendererState state) override;
    void ResetState() override;
    bool IsUsable() override;
    // colour
    LA::vec4 GetClearColor() override;
    LA::vec4 GetColorMask() override;
    void SetClearColor(const LA::vec4& colour) override;
    void SetColorMask(const LA::vec4& mask) override;
    // culling
    bool GetCullTest() override;
    CullFace GetCullFace() override;
    CullWinding GetCullWinding() override;
    void SetCullTest(bool enabled) override;
    void SetCullFace(CullFace face) override;
    void SetCullWinding(CullWinding winding) override;
    // depth triangle vertices we're defining them in a certain winding order that is either clockwise or counter-clockwise. Each triangle consists of 3 vertices and we specify those 3 vertices in a winding order as seen from the center of the triangle.
    bool GetDepthTest() override;
    DepthFunc GetDepthFunction() override;
    bool GetDepthMask() override;
    void SetDepthTest(bool enabled) override;
    void SetDepthFunction(DepthFunc func) override;
    void SetDepthMask(bool enabled) override;
    // rasterization
    float GetLineWidth() override;
    float GetPointSize() override;
    bool GetIsWireframe() override;
    void SetLineWidth(float width) override;
    void SetPointSize(float size) override;
    void SetIsWireframe(bool enabled) override;
    // bound objects
    std::shared_ptr<Shader> GetShader() override;
    void SetShader(std::shared_ptr<Shader> shader) override;

    /// --- Coordinate System Transformations ---
    // camera and screen transformations
    LA::mat4 GetProjection() override;
    LA::mat4 GetView() override;
    LA::mat4 GetModel() override;
    void SetProjection(const LA::mat4& proj) override;
    void SetView(const LA::mat4& view) override;
    // coordinate space transformations
    LA::mat4 PopTransform() override;
    void PushTransform(const LA::mat4& transform) override;
    void PushTranslate(float x, float y, float z) override;
    void PushTranslate(LA::vec3 translate) override;
    void PushRotate(float x, float y, float z) override;
    void PushRotate(LA::vec3 rotate) override;
    void PushScale(float x, float y, float z) override;
    void PushScale(LA::vec3 scale) override;
    
    /// --- Debug Info ---
    RenderStats GetRenderStats() override;

};

} // opengl

} // renderer

} // marathon
