#include "renderer/opengl/renderer.hpp"

namespace marathon {

namespace renderer {

namespace opengl {

/// --- Mesh Handling ---
/// TODO:
// support for a simple wireframe mode to convert to LINE versions of primitive types
// implement a better invalid state, as not uniform across all enums
const std::unordered_map<VertexAttributeFormat, GLenum> Renderer::s_vertexAttrFormatMap = {
    { VertexAttributeFormat::INVALID, 0 },
    { VertexAttributeFormat::HALF_FLOAT, GL_HALF_FLOAT },
    { VertexAttributeFormat::FLOAT, GL_FLOAT },
    { VertexAttributeFormat::DOUBLE, GL_DOUBLE },
    { VertexAttributeFormat::INT8, GL_BYTE },
    { VertexAttributeFormat::INT16, GL_SHORT },
    { VertexAttributeFormat::INT32, GL_INT },
    { VertexAttributeFormat::UINT8, GL_UNSIGNED_BYTE },
    { VertexAttributeFormat::UINT16, GL_UNSIGNED_SHORT },
    { VertexAttributeFormat::UINT32, GL_UNSIGNED_INT },
};

const std::unordered_map<PrimitiveType, GLenum> Renderer::s_primitiveMap = {
    { PrimitiveType::TRIANGLES, GL_TRIANGLES },
    { PrimitiveType::FAN, GL_TRIANGLE_FAN },
    { PrimitiveType::STRIP, GL_TRIANGLE_STRIP }
};

const std::unordered_map<IndexFormat, GLenum> Renderer::s_indexFormatMap = {
    { IndexFormat::UINT8, GL_UNSIGNED_BYTE },
    { IndexFormat::UINT16, GL_UNSIGNED_SHORT },
    { IndexFormat::UINT32, GL_UNSIGNED_INT },
    { IndexFormat::INVALID, 0 }
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

Renderer::Renderer() 
    : renderer::Renderer("marathon.renderer.opengl.Renderer") {}
Renderer::~Renderer() {
    // delete opengl resources
    /// CONSIDER: moving internal to the structs themselves
    for (auto& meshHandler : _meshHandlers) {
        glDeleteVertexArrays(1, &meshHandler.vao);
        glDeleteBuffers(1, &meshHandler.vbo);
        glDeleteBuffers(1, &meshHandler.ibo);
    }
    for (auto& shaderHandler : _shaderHandlers) {
        glDeleteProgram(shaderHandler.program);
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
/// TODO: currently does not consider if user shader is dirty
bool Renderer::ValidateShader(std::shared_ptr<Shader> shader, std::string& err) {
    int shaderHandlerIdx = FindOrCreateShaderHandler(shader);
    ShaderHandler& shaderHandler = _shaderHandlers[shaderHandlerIdx];
    if (shaderHandler.isValid) {
        return true;
    }
    err = shaderHandler.warnings;
    return false;
}

/// TODO: currently does not consider if user shader is dirty
bool Renderer::ValidateMesh(std::shared_ptr<Mesh> mesh, std::string& err) {
    int meshHandlerIdx = FindOrCreateMeshHandler(mesh);
    MeshHandler& meshHandler = _meshHandlers[meshHandlerIdx];
    if (meshHandler.isValid) {
        return true;
    }
    err = meshHandler.warnings;
    return false;
}


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
    /// TODO: implement
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
    glCullFace(s_cullFaceMap.at(face));
}
void Renderer::SetCullWinding(CullWinding winding) {
    _state.cullWinding = winding;
    glFrontFace(s_cullWindingMap.at(winding));
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
    glDepthFunc(s_depthFuncMap.at(func));
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
/// IMPORTANT NOTE TODO: current handler system ONLY allows for a single mesh/shader per handler
/// CONSIDER implications
/// No check of dirty flags atm
int Renderer::CreateMeshHandler(std::shared_ptr<Mesh> mesh) {
    // assert(vBuf != nullptr && "Vertex buffer must not be null");
    // assert(vCount > 0 && "Vertex count must be greater than 0");
    // assert(vAttrs.size() > 0 && "Vertex attributes must not be empty");

    int vertexCount = mesh->GetVertexCount();
    std::vector<VertexAttributeDescriptor>vertexAttrs = mesh->GetVertexAttributes();
    int vertexSize = mesh->GetVertexSize();
    const void* vertexData = mesh->GetVertexPtr();

    // NOTE: we still create opengl resources even if data is not uploaded/it remains empty
    // it is safe to do so, so :/
    GLuint vao = 0, vbo = 0, ibo = 0;
    std::string warnings = "";
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    if (vertexCount == 0)
        warnings += "No vertices defined\n";
    if (vertexAttrs.size() == 0)
        warnings += "No vertex attributes defined\n";
    if (vertexSize == 0)
        warnings += "Vertex size is 0 bytes\n";
    if (vertexData == nullptr)
        warnings += "Vertex data is nullptr\n";

    // create and bind vertex buffer
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexSize * vertexCount, vertexData, GL_STATIC_DRAW);

    /// TODO: more efficient to manually go through internal mesh data rather than repeatedly using
    /// iterative get methods
    for (int i = 0; i < vertexAttrs.size(); i++) {
        const auto& desc = vertexAttrs[i];
        int attrLoc = mesh->GetVertexAttributeLocation(desc.attribute);
        if (attrLoc == -1) {
            warnings += "Vertex attribute (" + std::to_string(i) + ") location invalid\n";
            continue;
        }
        
        GLenum attrType = s_vertexAttrFormatMap.at(desc.format);
        if (attrType == 0) {
            warnings += "Vertex attribute (" + std::to_string(i) + ") format invalid\n";
            continue;
        }
        int offset = mesh->GetVertexAttributeOffset(desc.attribute);

        glEnableVertexAttribArray(attrLoc);
        glVertexAttribPointer(attrLoc, desc.numComponents, attrType,
            GL_FALSE, vertexSize, (void*)offset);
    }

    // create and bind index buffer if set
    int indexCount = mesh->GetIndexCount();
    int indexSize = mesh->GetIndexSize();
    const void* indexData = mesh->GetIndexPtr();
    if (indexCount != 0 ) {
        if (indexSize == 0)
            warnings += "Index size is 0 bytes\n";
        if (indexData == nullptr)    
            warnings += "Index data is nullptr\n";

        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * indexSize, indexData, GL_STATIC_DRAW);
    } else {
        std::cout << "src/renderer/opengl/renderer.cpp: MESSAGE @ Renderer::CreateMeshHandler: no indices defined" << std::endl;
    }

    // unbind vao for safety
    glBindVertexArray(0);

    MeshHandler meshHandler = {
        .mesh = mesh,
        .vao = vao,
        .vbo = vbo,
        .ibo = ibo,
        .warnings = warnings,
        .isValid = vertexCount > 0 && vertexAttrs.size() > 0 && vertexSize > 0 && (indexCount == 0 || (indexCount > 0 && indexSize > 0))
    };
    _meshHandlers.push_back(meshHandler);
    return _meshHandlers.size() - 1;
}
int Renderer::FindOrCreateMeshHandler(std::shared_ptr<Mesh> mesh) {
    for (int i = 0; i < _meshHandlers.size(); i++) {
        if (_meshHandlers[i].mesh == mesh) {
            return i;
        }
    }
    return CreateMeshHandler(mesh);
}


/// --- Shader Stuff ---

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
        .shader = shader,
        .program = program,
        .warnings = warnings,
        .isValid = vSuccess && fSuccess && pSuccess
    };
    _shaderHandlers.push_back(shaderHandler);
    return _shaderHandlers.size() - 1;
}
int Renderer::FindOrCreateShaderHandler(std::shared_ptr<Shader> shader) {
    for (int i = 0; i < _shaderHandlers.size(); i++) {
        if (_shaderHandlers[i].shader == shader) {
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
    if (_shaderHandler != nullptr && _shaderHandler->shader == shader) {
        std::cout << "src/renderer/opengl/renderer.cpp: MESSAGE @ Renderer::SetShader: setting shader to already bound shader" << std::endl;
        return;
    }

    // check shader is valid
    std::string err = "";
    if (!ValidateShader(shader, err)) {
        std::cout << "src/renderer/opengl/renderer.cpp: MESSAGE @ Renderer::SetShader: can't bind shader to invalid shader" << std::endl;
        return;
    }
    // set and bind new shader
    int shaderHandlerIdx = FindOrCreateShaderHandler(shader);
    _shaderHandler = &_shaderHandlers.at(shaderHandlerIdx);
}

/// --- Shader Methods ---
bool Renderer::HasUniform(const std::string& key) {
    if (_shaderHandler == nullptr) {
        std::cout << "src/renderer/opengl/renderer.cpp: WARNING @ Renderer::HasUniform: no shader bound" << std::endl;
        return false;
    }
    std::string err;
    if (!ValidateShader(_shaderHandler->shader, err)) {
        std::cout << "src/renderer/opengl/renderer.cpp: WARNING @ Renderer::HasUniform: bound shader is invalid" << std::endl;
        return false;
    }
    return glGetUniformLocation(_shaderHandler->program, key.c_str()) != -1;
}

/// TODO: implement SetUniforms
/// currently implemented as direct uploads to gpu using bound shader
/// change implementation to store uniforms with shader that are uploaded to gpu at draw time
// single value uniforms
bool Renderer::SetUniform(const std::string& key, int value) {
    if (!HasUniform(key))
        return false;
    glUniform1i(glGetUniformLocation(_shaderHandler->program, key.c_str()), value);
    return true;
}
bool Renderer::SetUniform(const std::string& key, uint32_t value) {
    if (!HasUniform(key))
        return false;
    glUniform1ui(glGetUniformLocation(_shaderHandler->program, key.c_str()), value);
    return true;
}
bool Renderer::SetUniform(const std::string& key, float value) {
    if (!HasUniform(key))
        return false;
    glUniform1f(glGetUniformLocation(_shaderHandler->program, key.c_str()), value);
    return true;
}
bool Renderer::SetUniform(const std::string& key, double value) {
    if (!HasUniform(key))
        return false;
    glUniform1d(glGetUniformLocation(_shaderHandler->program, key.c_str()), value);
    return true;
}
// vector uniforms
bool Renderer::SetUniform(const std::string& key, const LA::vec2& v) {
    if (!HasUniform(key))
        return false;
    glUniform2f(glGetUniformLocation(_shaderHandler->program, key.c_str()), v.x, v.y);
    return true;
}
bool Renderer::SetUniform(const std::string& key, float x, float y) {
    if (!HasUniform(key))
        return false;
    glUniform2f(glGetUniformLocation(_shaderHandler->program, key.c_str()), x, y);
    return true;
}
bool Renderer::SetUniform(const std::string& key, const LA::vec3& v) {
    if (!HasUniform(key))
        return false;
    glUniform3f(glGetUniformLocation(_shaderHandler->program, key.c_str()), v.x, v.y, v.z);
    return true;
}
bool Renderer::SetUniform(const std::string& key, float x, float y, float z) {
    if (!HasUniform(key))
        return false;
    glUniform3f(glGetUniformLocation(_shaderHandler->program, key.c_str()), x, y, z);
    return true;
}
bool Renderer::SetUniform(const std::string& key, const LA::vec4& v) {
    if (!HasUniform(key))
        return false;
    glUniform4f(glGetUniformLocation(_shaderHandler->program, key.c_str()), v.x, v.y, v.z, v.w);
    return true;
}
bool Renderer::SetUniform(const std::string& key, float x, float y, float z, float w) {
    if (!HasUniform(key))
        return false;
    glUniform4f(glGetUniformLocation(_shaderHandler->program, key.c_str()), x, y, z, w);
    return true;
}
// matrix uniforms
bool Renderer::SetUniform(const std::string& key, const LA::mat2& m) {
    if (!HasUniform(key))
        return false;
    glUniformMatrix2fv(glGetUniformLocation(_shaderHandler->program, key.c_str()), 1, GL_FALSE, &m[0][0]);
    return true;
}
bool Renderer::SetUniform(const std::string& key, const LA::mat3& m) {
    if (!HasUniform(key))
        return false;
    glUniformMatrix3fv(glGetUniformLocation(_shaderHandler->program, key.c_str()), 1, GL_FALSE, &m[0][0]);
    return true;
}
bool Renderer::SetUniform(const std::string& key, const LA::mat4& m) {
    if (!HasUniform(key))
        return false;
    glUniformMatrix4fv(glGetUniformLocation(_shaderHandler->program, key.c_str()), 1, GL_FALSE, &m[0][0]);
    return true;
}


} // namespace opengl

} // namespace renderer

} // namespace marathon