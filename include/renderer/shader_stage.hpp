#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "tai/tai.hpp"

#define SHADER_INVALID  0
#define SHADER_VERTEX   1
#define SHADER_FRAGMENT 2
#define SHADER_COMPUTE  3
#define SHADER_GEOMETRY 4
#define SHADER_TESSELLATION_CONTROL     5
#define SHADER_TESSELLATION_EVALUATION  6

class ShaderStage : public Asset {
    private:
        std::string _source;
        int _stage = SHADER_INVALID;
        
    public:
        ShaderStage(std::string name="Default Shader Stage", const std::string& source="", int stage=SHADER_INVALID) :
            Asset(name),
            _source(source),
            _stage(stage) {}

        // source setters/getters
        std::string GetSource() {
            return _source;
        }

        void SetSource(std::string source) {
            _source = source;
        }

        // shader type getters/setters
        int GetStage() {
            return _stage;
        }

        void SetStage(int stage) {
            _stage = stage;
        }

};
