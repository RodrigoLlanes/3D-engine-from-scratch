//
// Created by Rodrigo Llanes on 28/09/2023.
//
#include "shaders/shader.hpp"


namespace Engine::Shaders {
    void Shader::interpolationWeights(Vector4f v1, Vector4f v2, Vector4f v3, Vector2f pos) {
        weights.x = ((v2.y - v3.y) * (pos.x - v3.x) + (v3.x - v2.x) * (pos.y - v3.y))/
                   ((v2.y - v3.y) * (v1.x - v3.x) + (v3.x - v2.x) * (pos.y - v3.y));


        weights.y = ((v3.y - v1.y) * (pos.x - v3.x) + (v1.x - v3.x) * (pos.y - v3.y))/
                   ((v2.y - v3.y) * (v1.x - v3.x) + (v3.x - v2.x) * (pos.y - v3.y));

        weights.z = 1.0f - weights.x - weights.y;
    }

    Vector4f Shader::interpolateV4f(Vector4f v1, Vector4f v2, Vector4f v3) {
        return {
                interpolateFloat(v1.x, v2.x, v3.x),
                interpolateFloat(v1.y, v2.y, v3.y),
                interpolateFloat(v1.z, v2.z, v3.z),
                interpolateFloat(v1.w, v2.w, v3.w)
        };
    }

    Vector4i Shader::interpolateV4i(Vector4i v1, Vector4i v2, Vector4i v3) {
        return {
                interpolateInt(v1.x, v2.x, v3.x),
                interpolateInt(v1.y, v2.y, v3.y),
                interpolateInt(v1.z, v2.z, v3.z),
                interpolateInt(v1.w, v2.w, v3.w)
        };
    }

    int Shader::interpolateInt(int i1, int i2, int i3) {
        return i1 * weights.x + i2 * weights.y + i3 * weights.z;
    }

    float Shader::interpolateFloat(float i1, float i2, float i3) {
        return i1 * weights.x + i2 * weights.y + i3 * weights.z;
    }
}
