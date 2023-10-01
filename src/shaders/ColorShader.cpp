//
// Created by Rodrigo Llanes on 28/09/2023.
//

#include <iostream>
#include "shaders/colorShader.hpp"

namespace Engine::Shaders {

    ColorShader::FShaderInput *ColorShader::vertex(ColorShader::VertexData* input) {
        return new ColorShader::FShaderInput(
                Vector4f(input->position.x, input->position.y, input->position.z, 1.0f),
                input->vertexColor
        );
    }

    void ColorShader::vertex(Shader::VertexData* i1, Shader::VertexData* i2, Shader::VertexData* i3) {
        auto* v1 = (ColorShader::VertexData*) i1;
        auto* v2 = (ColorShader::VertexData*) i2;
        auto* v3 = (ColorShader::VertexData*) i3;

        delete f1;
        delete f2;
        delete f3;

        f1 = vertex(v1);
        f2 = vertex(v2);
        f3 = vertex(v3);
    }

    Vector4i ColorShader::fragment(Vector2f pos) {
        interpolationWeights(f1->fragCoord, f2->fragCoord, f3->fragCoord, pos);

        return interpolateV4i(f1->color, f2->color, f3->color);
    }

}
