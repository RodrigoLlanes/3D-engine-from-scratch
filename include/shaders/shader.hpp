//
// Created by Rodrigo Llanes on 28/09/2023.
//

#ifndef GRAPHIC_ENGINE_SHADER_HPP
#define GRAPHIC_ENGINE_SHADER_HPP

#include "vectors.hpp"

namespace Engine::Shaders {

    class Shader {
    public:
        struct VertexData {
        };

        struct FShaderInput {
            Vector4f fragCoord;

            FShaderInput(Vector4f fragCoord) : fragCoord(fragCoord) {}
        };

        Vector3f  weights;
        virtual FShaderInput* getF1() = 0;
        virtual FShaderInput* getF2() = 0;
        virtual FShaderInput* getF3() = 0;

        virtual void vertex(VertexData* v1, VertexData* v2, VertexData* v3) = 0;
        virtual Vector4i fragment(Vector2f pos) = 0;

        void interpolationWeights(Vector4f v1, Vector4f v2, Vector4f v3, Vector2f pos);
        Vector4f interpolateV4f(Vector4f v1, Vector4f v2, Vector4f v3);
        Vector4i interpolateV4i(Vector4i v1, Vector4i v2, Vector4i v3);
        int interpolateInt(int i1, int i2, int i3);
        float interpolateFloat(float i1, float i2, float i3);
    };

}

#endif //GRAPHIC_ENGINE_SHADER_HPP
