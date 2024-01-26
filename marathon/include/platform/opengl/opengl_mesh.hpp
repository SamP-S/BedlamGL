#pragma once

#include <vector>
#include <iostream>
#include <string>

#include "platform/opengl/opengl.hpp"
#include "renderer/mesh.hpp"

//// TODO:
// think about renaming position to "vertex" or "fragment" for coordinate space to be inherant in the name
// modify to use an array of buffers instead of having 8 fixed buffers, glGenBuffers(n, &buffers)
// support uv4-uv7, bitangents
// implement using VertexAttribute configuration structs to make it flexible

enum OpenGLAttribLocs : uint32_t {
    POSITION = 0,
    NORMAL = 1,
    TANGENT = 2,
    COLOUR = 3,
    UV0 = 4,
    UV1 = 5,
    UV2 = 6,
    UV3 = 7
};

class OpenGLMesh : public Mesh {
    public:
        // Set if no vertex attribute data
        // Otherwise leave as 0
        uint32_t vertexCount = 0;
        

    // todo add assimp loaded meshes
        OpenGLMesh(std::string name="Mesh")
            : Mesh(name) {}

        ~OpenGLMesh() {
            // delete all opengl resources/buffers
            glDeleteVertexArrays(1, &_vaBuffer);
            glDeleteBuffers(1, &_vertexBuffer);
            glDeleteBuffers(1, &_normalBuffer);
            glDeleteBuffers(1, &_tangentBuffer);
            glDeleteBuffers(1, &_colourBuffer);
            glDeleteBuffers(1, &_indexBuffer);
            glDeleteBuffers(1, &_uv0Buffer);
            glDeleteBuffers(1, &_uv1Buffer);
            glDeleteBuffers(1, &_uv2Buffer);
            glDeleteBuffers(1, &_uv3Buffer);
        }

        bool IsUsable() override {
            return _isGenerated;
        }
        
        void Draw() override {
            if (!_isGenerated) {
                Generate();
                if (!_isGenerated) {
                    return;
                }
            }

            // draw call depending on if using indicie buffer
            glBindVertexArray(_vaBuffer);
            if (_isIndexed) {
                glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
            } else {
                glDrawArrays(GL_TRIANGLES, 0, vertices.size());
            }
            glBindVertexArray(0);
        }

    private:
        bool _isGenerated = false;
        bool _isIndexed = false;

        uint32_t _vertexBuffer, _normalBuffer, _tangentBuffer, _colourBuffer, _indexBuffer = 0;
        uint32_t _uv0Buffer, _uv1Buffer, _uv2Buffer, _uv3Buffer = 0;
        uint32_t _vaBuffer = 0;

        template <typename T>
        uint32_t GenerateBuffer(GLenum target, const std::vector<T>& data) {
            uint32_t buf = 0;
            glGenBuffers(1, &buf);
            glBindBuffer(target, buf);
            glBufferData(target, data.size() * sizeof(T), &data.front(), GL_STATIC_DRAW);
            return buf;
        }

        void Generate() {
            // check if generation needed
            if (_isGenerated) {
                return;
            }

            // create vao
            _isGenerated = true;
            glGenVertexArrays(1, &_vaBuffer);
            glBindVertexArray(_vaBuffer);

            // vertices
            if (vertices.size() == 0) {
                std::cout << "DEBUG (Mesh): Trying to generate buffers for mesh with no verices." << std::endl;
            } else {
                _vertexBuffer = GenerateBuffer<LA::vec3>(GL_ARRAY_BUFFER, vertices);
                glVertexAttribPointer(OpenGLAttribLocs::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(LA::vec3), (void*)0);
                glEnableVertexAttribArray(OpenGLAttribLocs::POSITION);
            }
            
            if (indices.size() == 0) {
                _isIndexed = false;
            } else {
                _isIndexed = true;
                _indexBuffer = GenerateBuffer<uint32_t>(GL_ELEMENT_ARRAY_BUFFER, indices);
            }

            // normals
            if (normals.size() > 0) {
                _normalBuffer = GenerateBuffer<LA::vec3>(GL_ARRAY_BUFFER, normals);
                glVertexAttribPointer(OpenGLAttribLocs::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(LA::vec3), (void*)0);
                glEnableVertexAttribArray(OpenGLAttribLocs::NORMAL);
            }
            // tangents
            if (tangents.size() > 0) {
                _tangentBuffer = GenerateBuffer<LA::vec4>(GL_ARRAY_BUFFER, tangents);
                glVertexAttribPointer(OpenGLAttribLocs::TANGENT, 4, GL_FLOAT, GL_FALSE, sizeof(LA::vec4), (void*)0);
                glEnableVertexAttribArray(OpenGLAttribLocs::TANGENT);
            }
            // colours
            if (colours.size() > 0) {
                _colourBuffer = GenerateBuffer<LA::vec4>(GL_ARRAY_BUFFER, colours);
                glVertexAttribPointer(OpenGLAttribLocs::COLOUR, 4, GL_FLOAT, GL_FALSE, sizeof(LA::vec4), (void*)0);
                glEnableVertexAttribArray(OpenGLAttribLocs::COLOUR);
            }

            // textures
            if (uv0.size() > 0) {
                _uv0Buffer = GenerateBuffer<LA::vec2>(GL_ARRAY_BUFFER, uv0);
                glVertexAttribPointer(OpenGLAttribLocs::UV0, 2, GL_FLOAT, GL_FALSE, sizeof(LA::vec2), (void*)0);
                glEnableVertexAttribArray(OpenGLAttribLocs::UV0);
            }
            if (uv1.size() > 0) {
                _uv1Buffer = GenerateBuffer<LA::vec2>(GL_ARRAY_BUFFER, uv1);
                glVertexAttribPointer(OpenGLAttribLocs::UV1, 2, GL_FLOAT, GL_FALSE, sizeof(LA::vec2), (void*)0);
                glEnableVertexAttribArray(OpenGLAttribLocs::UV1);
            }
            if (uv2.size() > 0) {
                _uv2Buffer = GenerateBuffer<LA::vec2>(GL_ARRAY_BUFFER, uv2);
                glVertexAttribPointer(OpenGLAttribLocs::UV2, 2, GL_FLOAT, GL_FALSE, sizeof(LA::vec2), (void*)0);
                glEnableVertexAttribArray(OpenGLAttribLocs::UV2);
            }
            if (uv3.size() > 0) {
                _uv3Buffer = GenerateBuffer<LA::vec2>(GL_ARRAY_BUFFER, uv3);
                glVertexAttribPointer(OpenGLAttribLocs::UV3, 2, GL_FLOAT, GL_FALSE, sizeof(LA::vec2), (void*)0);
                glEnableVertexAttribArray(OpenGLAttribLocs::UV3);
            }
            glBindVertexArray(0);
        }
};
