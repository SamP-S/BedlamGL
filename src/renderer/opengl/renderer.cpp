#include "renderer/opengl/renderer.hpp"
#include "renderer/opengl/shader.hpp"
#include "renderer/opengl/buffer.hpp"
#include "renderer/opengl/mesh.hpp"

namespace marathon {

namespace renderer {

namespace opengl {

Renderer::Renderer() 
    : renderer::Renderer("marathon.renderer.opengl.Renderer") {}
Renderer::~Renderer() {}

// module interface
bool Renderer::Boot() {
    _active = true;
    return true;
}
bool Renderer::Shutdown() {
    _active = false;
    return true;
}

/// --- Factories ---
std::shared_ptr<renderer::Buffer> Renderer::CreateBuffer(void* data, size_t size, BufferTarget target, BufferUsage usage) {
    return std::shared_ptr<renderer::Buffer>(new opengl::Buffer(data, size, target, usage));
}
std::shared_ptr<renderer::Mesh> Renderer::CreateMesh(int vCount, size_t vSize, std::shared_ptr<renderer::Buffer> vBuf, std::vector<VertexAttribute> vAttrs, PrimitiveType primitive) {
    return std::shared_ptr<renderer::Mesh>(new opengl::Mesh(vCount, vSize, vBuf, vAttrs, primitive));
}
std::shared_ptr<renderer::Shader> Renderer::CreateShader(const std::string& vSrc, const std::string& fSrc) {
    return std::shared_ptr<renderer::Shader>(new opengl::Shader(vSrc, fSrc));
}

// validation
bool Renderer::ValidateShaderCode(const std::string code, ShaderType stageType, std::string& err) {}


/// --- Drawing ---
// clear active canvas/screen of all color, depth, and stencil buffers
void Renderer::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
// clear active canvas/screen according to bools args
void Renderer::Clear(bool clearColor, bool clearStencil, bool clearDepth) {
    GLbitfield mask = 0;
    if (clearColor) mask |= GL_COLOR_BUFFER_BIT;
    if (clearStencil) mask |= GL_STENCIL_BUFFER_BIT;
    if (clearDepth) mask |= GL_DEPTH_BUFFER_BIT;
    glClear(mask);
}

void Renderer::Draw(Drawable& d) {
    d.Draw(*this);
    _stats.drawCalls++;
}

/// --- State Management ---
void Renderer::SetState(RendererState state) {

}
void Renderer::ResetState() {
    SetState(RendererState());
}
bool Renderer::IsUsable() {
    /// TODO: validate the module
    return true;
}
// colour
LA::vec4 Renderer::GetClearColor() {
    return _state.clearColor;
}
LA::vec4 Renderer::GetColorMask() {
    return _state.colorMask;
}
void Renderer::SetClearColor(const LA::vec4& colour) {
    _state.clearColor = colour;
    glClearColor(colour.r, colour.g, colour.b, colour.a);
}
void Renderer::SetColorMask(const LA::vec4& mask) {
    _state.colorMask = mask;
    glColorMask(mask.r != 0.0f, mask.g != 0.0f, mask.b != 0.0f, mask.a != 0.0f);
}
// culling
bool Renderer::GetCullTest() {
    return _state.cullTest;
}
CullFace Renderer::GetCullFace() {
    return _state.cullFace;
}
CullWinding Renderer::GetCullWinding() {
    return _state.cullWinding;
}
void Renderer::SetCullTest(bool enabled) {
    _state.cullTest = enabled;
    if (enabled) glEnable(GL_CULL_FACE);
    else glDisable(GL_CULL_FACE);
}
void Renderer::SetCullFace(CullFace face) {
    _state.cullFace = face;
    glCullFace(_cullFaceMap[face]);
}
void Renderer::SetCullWinding(CullWinding winding) {
    _state.cullWinding = winding;
    glFrontFace(_cullWindingMap[winding]);
}
// depth triangle vertices we're defining them in a certain winding order that is either clockwise or counter-clockwise. Each triangle consists of 3 vertices and we specify those 3 vertices in a winding order as seen from the center of the triangle.
bool Renderer::GetDepthTest() {
    return _state.depthTest;
}
DepthFunc Renderer::GetDepthFunction() {
    return _state.depthFunc;
}
bool Renderer::GetDepthMask() {
    return _state.depthMask;
}
void Renderer::SetDepthTest(bool enabled) {
    _state.depthTest = enabled;
    if (enabled) glEnable(GL_DEPTH_TEST);
    else glDisable(GL_DEPTH_TEST);
}
void Renderer::SetDepthFunction(DepthFunc func) {
    _state.depthFunc = func;
    glDepthFunc(_depthFuncMap[func]);
}
void Renderer::SetDepthMask(bool enabled) {
    _state.depthMask = enabled;
    glDepthMask(enabled);
}
// rasterization
float Renderer::GetLineWidth() {
    return _state.lineWidth;
}
float Renderer::GetPointSize() {
    return _state.pointSize;
}
bool Renderer::GetIsWireframe() {
    return _state.isWireframe;
}
void Renderer::SetLineWidth(float width) {
    _state.lineWidth = width;
    glLineWidth(width);
}
void Renderer::SetPointSize(float size) {
    _state.pointSize = size;
    glPointSize(size);
}
void Renderer::SetIsWireframe(bool enabled) {
    _state.isWireframe = enabled;
    if (enabled) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
// bound objects
std::shared_ptr<renderer::Shader> Renderer::GetShader() {
    return _shader;
}
/// TODO:
// set nullptr as shader should mean use a default not just shit the bed 
void Renderer::SetShader(std::shared_ptr<renderer::Shader> shader) {
    // skip if same
    if (_shader == shader)
        return;

    // unbind previous shader, if any
    if (_shader != nullptr) {
        _shader->Unbind();
    }

    // set and bind new shader
    _shader = shader;
    if (_shader != nullptr) {
        _shader->Bind();
    }
}

} // namespace opengl

} // namespace renderer

} // namespace marathon