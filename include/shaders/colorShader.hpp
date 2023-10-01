//
// Created by Rodrigo Llanes on 28/09/2023.
//

#ifndef GRAPHIC_ENGINE_COLORSHADER_HPP
#define GRAPHIC_ENGINE_COLORSHADER_HPP

#include <utility>

#include "shader.hpp"


namespace Engine::Shaders {

    class ColorShader : public Shader {
    public:
        struct VertexData : Shader::VertexData {
            Vector3f position;
            Vector4i vertexColor;

            VertexData(Vector3f position, Vector4i color) : position(position), vertexColor(color) {}
        };

        struct FShaderInput : Shader::FShaderInput {
            Vector4i color;

            FShaderInput(Vector4f fragCoord, Vector4i color) : Shader::FShaderInput(fragCoord), color(color) {}
        };

        FShaderInput *f1 = nullptr, *f2 = nullptr, *f3 = nullptr;
        Shader::FShaderInput* getF1() override { return f1; };
        Shader::FShaderInput* getF2() override { return f2; };
        Shader::FShaderInput* getF3() override { return f3; };

        void vertex(Shader::VertexData* v1, Shader::VertexData* v2, Shader::VertexData* v3) override;
        Vector4i fragment(Vector2f pos) override;

    private:
        FShaderInput* vertex(VertexData* v);
    };
}

#endif //GRAPHIC_ENGINE_COLORSHADER_HPP
