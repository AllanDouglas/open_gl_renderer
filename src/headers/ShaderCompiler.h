#pragma once
#include "./includes.h"

class ShaderCompiler
{
public:
    GLuint LoadShaders(const char *VertextShaderFile, const char *fragmentShaderFile);

private:
    void CheckShader(GLuint shaderId);
};
