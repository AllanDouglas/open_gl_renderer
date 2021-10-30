#include <array>
#include <cassert>
#include "includes.h"
#include "RenderInfo.h"
#include "FlyCamera.h"
#include "ShaderCompiler.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

const int Width = 800;
const int Height = 600;

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Color;
	glm::vec2 UV;
};

GLuint LoadTexture(const char *textureFile)
{
	stbi_set_flip_vertically_on_load(true);

	int textureWidth = 0;
	int textureHeigth = 0;
	int numberComponents = 0;
	auto textureData = stbi_load(textureFile, &textureWidth, &textureHeigth, &numberComponents, 3);

	assert(textureData);

	// STARTING - load to video card
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeigth, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);

	// filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_MIPMAP);

	// wraping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// create mipmap
	glGenerateMipmap(GL_TEXTURE_2D);

	// remove txture
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(textureData);

	return textureId;
}

ShaderCompiler shaderCompiler;
FlyCamera camera{Width, Height};

int main()
{

	assert(glfwInit() == GLFW_TRUE);

	GLFWwindow *Window = glfwCreateWindow(Width, Height, "OpenGL", nullptr, nullptr);
	assert(Window);

	glfwMakeContextCurrent(Window);

	assert(glewInit() == GLEW_OK);

	RenderInfo info;
	info.ShowInfo();

	auto shaderID = shaderCompiler.LoadShaders("shaders/triangle_vert.glsl", "shaders/triangle_frag.glsl");
	auto textureID = LoadTexture("textures/earth_2k.jpg");

	std::array<Vertex, 6> quad = {
		Vertex{glm::vec3{-1, -1, 0}, glm::vec3{1.0f, 0.0f, 0.0f}, glm::vec2(0.0f, 0.0f)}, // bot left
		Vertex{glm::vec3{1, -1, 0}, glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec2(1.0f, 0.0f)},  // bot right
		Vertex{glm::vec3{-1, 1, 0}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec2(0.0f, 1.0f)},  // top left

		Vertex{glm::vec3{1, -1, 0}, glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec2(1.0f, 0.0f)}, // bot right
		Vertex{glm::vec3{1, 1, 0}, glm::vec3{1.0f, 0.0f, 0.0f}, glm::vec2(1.0f, 1.0f)},	 // top right
		Vertex{glm::vec3{-1, 1, 0}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec2(0.0f, 1.0f)}, // top left
	};

	glm::mat4 ModelMatrix = glm::identity<glm::mat4>();

	GLuint vertexBuffer;

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad.data(), GL_STATIC_DRAW);

	glClearColor(0.3f, .3f, .3f, 1);

	while (!glfwWindowShouldClose(Window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 mvp = camera.GetViewProjection() * ModelMatrix;

		glUseProgram(shaderID);

		auto MVPLocation = glGetUniformLocation(shaderID, "MVP");
		glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(mvp));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		auto TextureLocation = glGetUniformLocation(shaderID, "TextureSampler");
		glUniform1i(TextureLocation, 0);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);										   // position
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, Color))); // color
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, UV)));	   // UV

		glDrawArrays(GL_TRIANGLES, 0, quad.size());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glUseProgram(0);
		glfwPollEvents();
		glfwSwapBuffers(Window);
	}

	glfwTerminate();
	return 0;
}
