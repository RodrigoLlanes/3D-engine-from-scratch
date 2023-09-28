//
// Created by Rodrigo Llanes on 28/09/2023.
//

#ifndef GRAPHIC_ENGINE_SHADER_HPP
#define GRAPHIC_ENGINE_SHADER_HPP

#include <Eigen/Dense>

using Eigen::Vector4f;
using Eigen::Vector3f;
using Eigen::Vector2f;

struct ShaderUniforms {
};

struct VShaderInput {
};

struct FShaderInput {
    Vector4f fragCoord;
};

class Shader {
public:
    FShaderInput vertex(ShaderUniforms uniforms, VShaderInput input);
    Vector4f fragment(ShaderUniforms uniforms, FShaderInput input);
    FShaderInput interpolate(FShaderInput v1, FShaderInput v2, FShaderInput v3, Vector3f pos);
};

#endif //GRAPHIC_ENGINE_SHADER_HPP
