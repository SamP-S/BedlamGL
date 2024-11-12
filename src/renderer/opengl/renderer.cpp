#include "renderer/opengl/renderer.hpp"

namespace marathon {

namespace renderer {

namespace opengl {

/// --- Mesh Handling ---
/// TODO:
// support for a simple wireframe mode to convert to LINE versions of primitive types
const std::unordered_map<AttributeType, GLenum> Renderer::s_attrTypeMap = {
    { AttributeType::FLOAT, GL_FLOAT },
    { AttributeType::UINT8, GL_UNSIGNED_BYTE },
    { AttributeType::UINT16, GL_UNSIGNED_SHORT },
    { AttributeType::UINT32, GL_UNSIGNED_INT }
};

const std::unordered_map<PrimitiveType, GLenum> Renderer::s_primitiveMap = {
    { PrimitiveType::TRIANGLES, GL_TRIANGLES },
    { PrimitiveType::FAN, GL_TRIANGLE_FAN },
    { PrimitiveType::STRIP, GL_TRIANGLE_STRIP }
};

const std::unordered_map<IndexType, GLenum> Renderer::s_indexTypeMap = {
    { IndexType::UINT8, GL_UNSIGNED_BYTE },
    { IndexType::UINT16, GL_UNSIGNED_SHORT },
    { IndexType::UINT32, GL_UNSIGNED_INT }
};

const std::unordered_map<CullFace, GLenum> Renderer::s_cullFaceMap = {
    {CullFace::FRONT, GL_FRONT},
    {CullFace::BACK, GL_BACK},
    {CullFace::FRONT_AND_BACK, GL_FRONT_AND_BACK}
};

const std::unordered_map<CullWinding, GLenum> Renderer::s_cullWindingMap = {
    {CullWinding::CLOCKWISE, GL_CW},
    {CullWinding::COUNTER_CLOCKWISE, GL_CCW}
};

const std::unordered_map<DepthFunc, GLenum> Renderer::s_depthFuncMap = {
    {DepthFunc::NEVER, GL_NEVER},
    {DepthFunc::ALWAYS, GL_ALWAYS},
    {DepthFunc::EQUAL, GL_EQUAL},
    {DepthFunc::NOT_EQUAL, GL_NOTEQUAL},
    {DepthFunc::LESS, GL_LESS},
    {DepthFunc::LESS_EQUAL, GL_LEQUAL},
    {DepthFunc::GREATER, GL_GREATER},
    {DepthFunc::GREATER_EQUAL, GL_GEQUAL}
};

const std::unordered_map<ShaderType, GLenum> Renderer::s_shaderTypeMap = {
    { ShaderType::VERTEX, GL_VERTEX_SHADER },
    { ShaderType::FRAGMENT, GL_FRAGMENT_SHADER }
};

Renderer::Renderer() 
    : renderer::Renderer("marathon.renderer.opengl.Renderer") {}
Renderer::~Renderer() {
    // delete opengl resources
    for (auto& mesh : _meshes) {
        if (mesh != nullptr) {
            glDeleteVertexArrays(1, &mesh->vao);
            glDeleteBuffers(1, &mesh->vbo);
            glDeleteBuffers(1, &mesh->ibo);
        }
    }
    for (auto& shader : _shaders) {
        if (shader != nullptr) {
            glDeleteProgram(shader->program);
        }
    }
}

// module interface
bool Renderer::Boot() {
    _active = true;
    return true;
}
bool Renderer::Shutdown() {
    _active = false;
    return true;
}


// validation
bool Renderer::ValidateShader(std::shared_ptr<Shader> shader, std::string& err) {}
bool Renderer::ValidateMesh(std::shared_ptr<Mesh> mesh, std::string& err) {}


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

void Renderer::Draw(Mesh& d) {
    std::cout << "renderer/opengl/renderer.cpp: draw call, PLEASE IMPLEMENT" << std::endl;
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

/// --- Mesh Stuff ---
int Renderer::CreateMeshHandler(std::shared_ptr<Mesh> mesh) {
    std::cout << "src/renderer/opengl/renderer.cpp: CreateMeshHandler() not implemented" << std::endl;
    return 0;
}
int Renderer::FindOrCreateMeshHandler(std::shared_ptr<Mesh> mesh) {
    std::cout << "src/renderer/opengl/renderer.cpp: FindOrCreateMeshHandler() not implemented" << std::endl;
    return 0;
}


/// --- Shader Stuff ---
bool Renderer::CheckBoundShader() {
    if (_shaderHandler == nullptr) {
        std::cout << "src/renderer/opengl/renderer.cpp: WARNING @ Renderer::CheckBoundShader: no shader bound" << std::endl;
        return false;
    }
    if (!_shaderHandler->isValid) {
        std::cout << "src/renderer/opengl/renderer.cpp: WARNING @ Renderer::CheckBoundShader: shader is invalid" << std::endl;
        return false;
    }
    return true;
}

/// TODO: consider how to handle shader warnings and forcing the user to deal with bad shaders
int Renderer::CreateShaderHandler(std::shared_ptr<Shader> shader) {
    GLuint program = glCreateProgram();
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    const char* vSrcC = shader->GetVertexSource().c_str();
    const char* fSrcC = shader->GetFragmentSource().c_str();

    glShaderSource(vShader, 1, &vSrcC, nullptr);
    glShaderSource(fShader, 1, &fSrcC, nullptr);

    glCompileShader(vShader);
    glCompileShader(fShader);

    GLint vSuccess;
    std::string warnings = "";
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &vSuccess);
    if (!vSuccess) {
        GLchar infoLog[512];
        glGetShaderInfoLog(vShader, 512, nullptr, infoLog);
        warnings += "Vertex shader compilation failed: \n" + std::string(infoLog) + "\n\n";
    }

    GLint fSuccess;
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &fSuccess);
    if (!fSuccess) {
        GLchar infoLog[512];
        glGetShaderInfoLog(fShader, 512, nullptr, infoLog);
        warnings += "Fragment shader compilation failed: \n" + std::string(infoLog) + "\n\n";
    }

    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    glLinkProgram(program);

    GLint pSuccess;
    glGetProgramiv(program, GL_LINK_STATUS, &pSuccess);
    if (!pSuccess) {
        GLchar infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        warnings += "Shader program linking failed: \n" + std::string(infoLog) + "\n\n";
    }

    glDeleteShader(vShader);
    glDeleteShader(fShader);

    ShaderHandler shaderHandler = {
        .program = program,
        .warnings = warnings,
        .shader = shader,
        .isValid = vSuccess && fSuccess && pSuccess
    };
    _shaderHandlers.push_back(shaderHandler);
    return _shaderHandlers.size() - 1;
}
int Renderer::FindOrCreateShaderHandler(std::shared_ptr<Shader> shader) {
    for (int i = 0; i < _shaderHandlers.size(); i++) {
        if (_shaderHandlers[i]->shader == shader) {
            return i;
        }
    }
    return CreateShaderHandler(shader);
}

// bound objects
std::shared_ptr<renderer::Shader> Renderer::GetShader() {
    if (_shaderHandler == nullptr)
        return nullptr;
    return _shaderHandler->shader;
}
/// TODO:
// set nullptr as shader should mean use a default not just shit the bed 
void Renderer::SetShader(std::shared_ptr<renderer::Shader> shader) {
    if (shader == nullptr) {
        std::cout << "src/renderer/opengl/renderer.cpp: WARNING @ Renderer::SetShader: setting shader null, won't be able to draw" << std::endl;
        _shaderHandler = nullptr;
        glUseProgram(0);
        return;
    }

    // skip if same
    if (_shaderHandlers != nullptr && _shaderHandler->shader == shader) {
        std::cout << "src/renderer/opengl/renderer.cpp: MESSAGE @ Renderer::SetShader: setting shader to already bound shader" << std::endl;
        return;
    }

    // set and bind new shader
    _shader = shader;
    if (_shader != nullptr) {
       // glUseProgram()
    } else {
        std::cout << "src/renderer/opengl/renderer.cpp: WARNING @ Renderer::SetShader: shader arg is null" << std::endl;
    }
}

/// --- Shader Methods ---
bool Renderer::HasUniform(const std::string& key) const {
    if (!CheckBoundShader)
        return false;
    return glGetUniformLocation(_shaderHandler->program, key.c_str()) != -1;
}

/// TODO: implement SetUniforms
/// currently implemented as direct uploads to gpu using bound shader
/// change implementation to store uniforms with shader that are uploaded to gpu at draw time
// single value uniforms
bool Renderer::SetUniform(const std::string& key, int value) const {
    if (!HasUniform(key))
        return false;
    glUniform1i(glGetUniformLocation(_shaderHandler->program, key.c_str()), value);
    return true;
}
bool Renderer::SetUniform(const std::string& key, uint32_t value) const {
    if (!HasUniform(key))
        return false;
    glUniform1ui(glGetUniformLocation(_shaderHandler->program, key.c_str()), value);
    return true;
}
bool Renderer::SetUniform(const std::string& key, float value) const {
    if (!HasUniform(key))
        return false;
    glUniform1f(glGetUniformLocation(_shaderHandler->program, key.c_str()), value);
    return true;
}
bool Renderer::SetUniform(const std::string& key, double value) const {
    if (!HasUniform(key))
        return false;
    glUniform1d(glGetUniformLocation(_shaderHandler->program, key.c_str()), value);
    return true;
}
// vector uniforms
void Renderer::SetUniform(const std::string& key, const LA::vec2& v) const {
    std::cout << "src/renderer/opengl/renderer.cpp: SetUniform() not implemented" << std::endl;
    return false;
}
void Renderer::SetUniform(const std::string& key, float x, float y) const {
    std::cout << "src/renderer/opengl/renderer.cpp: SetUniform() not implemented" << std::endl;
    return false;
}
void Renderer::SetUniform(const std::string& key, const LA::vec3& v) const {
    std::cout << "src/renderer/opengl/renderer.cpp: SetUniform() not implemented" << std::endl;
    return false;
}
void Renderer::SetUniform(const std::string& key, float x, float y, float z) const {
    std::cout << "src/renderer/opengl/renderer.cpp: SetUniform() not implemented" << std::endl;
    return false;
}
void Renderer::SetUniform(const std::string& key, const LA::vec4& v) const {
    std::cout << "src/renderer/opengl/renderer.cpp: SetUniform() not implemented" << std::endl;
    return false;
}
void Renderer::SetUniform(const std::string& key, float x, float y, float z, float w) const {
    std::cout << "src/renderer/opengl/renderer.cpp: SetUniform() not implemented" << std::endl;
    return false;
}
// matrix uniforms
void Renderer::SetUniform(const std::string& key, const LA::mat2& m) const {
    std::cout << "src/renderer/opengl/renderer.cpp: SetUniform() not implemented" << std::endl;
    return false;
}
void Renderer::SetUniform(const std::string& key, const LA::mat3& m) const {
    std::cout << "src/renderer/opengl/renderer.cpp: SetUniform() not implemented" << std::endl;
    return false;
}
void Renderer::SetUniform(const std::string& key, const LA::mat4& m) const {
    std::cout << "src/renderer/opengl/renderer.cpp: SetUniform() not implemented" << std::endl;
    return false;
}


} // namespace opengl

} // namespace renderer

} // namespace marathon