#include "Utils.h"
#include "ShaderCompiler.h"

void ShaderCompiler::CheckShader(GLuint shaderId)
{
	GLint result = GL_TRUE;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		// log buffer size
		GLint logLength = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0)
		{
			std::string ShaderLog(logLength, '\0');
			glGetShaderInfoLog(shaderId, logLength, nullptr, &ShaderLog[0]);
			std::cout << "SHADER_COMPILE_ERROR: " << std::endl;
			std::cout << ShaderLog << std::endl;
			assert(false);
		}
	}
}

GLuint ShaderCompiler::LoadShaders(const char *VertextShaderFile, const char *fragmentShaderFile)
{
    auto vertex = ReadFile(VertextShaderFile);
    auto fragment = ReadFile(fragmentShaderFile);

    assert(vertex.empty() == false);
    assert(fragment.empty() == false);

    auto vertexID = glCreateShader(GL_VERTEX_SHADER);
    auto fragmentID = glCreateShader(GL_FRAGMENT_SHADER);

    auto vertexSource = vertex.c_str();
    glShaderSource(vertexID, 1, &vertexSource, nullptr);
    glCompileShader(vertexID);
    CheckShader(vertexID);

    auto fragmentSource = fragment.c_str();
    glShaderSource(fragmentID, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentID);
    CheckShader(fragmentID);

    auto programID = glCreateProgram();
    glAttachShader(programID, vertexID);
    glAttachShader(programID, fragmentID);
    glLinkProgram(programID);

    auto result = GL_TRUE;
    glGetProgramiv(programID, GL_LINK_STATUS, &result);

    if (result == GL_FALSE)
    {
        // log error
        GLint logLength = 0;
        std::cout << "LINK SHADER ERROR" << std::endl;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0)
        {
            std::string ShaderLog(logLength, '\0');
            glGetProgramInfoLog(programID, logLength, nullptr, &ShaderLog[0]);
            std::cout << ShaderLog << std::endl;
        }
        assert(false);
    }

    glDetachShader(programID, vertexID);
    glDetachShader(programID, fragmentID);
    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);

    return programID;
}