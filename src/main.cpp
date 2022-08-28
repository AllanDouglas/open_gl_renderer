#include <array>
#include <cassert>
#include "headers/includes.h"
#include "headers/RenderInfo.h"
#include "headers/FlyCamera.h"
#include "headers/ShaderCompiler.h"
#include "headers/TextureLoader.h"

const int Width = 800;
const int Height = 600;

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Color;
	glm::vec2 UV;
};

GLuint LoadTexture(const char* textureFile)
{
	return TextureLoader::LoadTexture(textureFile);
}

GLuint LoadGeometry()
{
	std::array<Vertex, 6> quad = {
		Vertex{glm::vec3{-1, -1, 0}, glm::vec3{1.0f, 0.0f, 0.0f}, glm::vec2(0.0f, 0.0f)}, // bot left
		Vertex{glm::vec3{1, -1, 0}, glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec2(1.0f, 0.0f)},  // bot right
		Vertex{glm::vec3{1, 1, 0}, glm::vec3{1.0f, 0.0f, 0.0f}, glm::vec2(1.0f, 1.0f)},	  // top right
		Vertex{glm::vec3{-1, 1, 0}, glm::vec3{0.0f, 1.0f, 0.0f}, glm::vec2(0.0f, 1.0f)},  // top left
	};

	std::array<glm::ivec3, 2> indexes = {
		glm::ivec3{0, 1, 2},
		glm::ivec3{3, 2, 0},
	};

	GLuint vertexBuffer;
	// create vertex buffer id
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad.data(), GL_STATIC_DRAW);

	GLuint elementBuffer;

	// create element buffer id
	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes.data(), GL_STATIC_DRAW);

	// create VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);

	// select VAO
	glBindVertexArray(vao);

	// send buffers to GPU
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);										   // position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Color))); // color
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, UV)));	   // UV

	glBindVertexArray(0);
	return vao;
}



int main()
{

	assert(glfwInit() == GLFW_TRUE);

	GLFWwindow* Window = glfwCreateWindow(Width, Height, "OpenGL", nullptr, nullptr);
	assert(Window);

	glfwMakeContextCurrent(Window);

	assert(glewInit() == GLEW_OK);

	RenderInfo info;
	info.ShowInfo();

	ShaderCompiler shaderCompiler;
	FlyCamera camera{ glm::vec3(0.0f,0.0f,5.0f), glm::vec3(0.0f,0.0f,-1.0f), glm::vec3(0.0f,1.0f,0.0f), 25.0f,Width / Height,1.0f,1000.0f };

	glm::mat4 ModelMatrix = glm::identity<glm::mat4>();
	auto shaderID = shaderCompiler.LoadShaders("shaders/triangle_vert.glsl", "shaders/triangle_frag.glsl");
	auto textureID = LoadTexture("textures/earth_2k.jpg");

	auto quadGemoetry = LoadGeometry();

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

		glBindVertexArray(quadGemoetry);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(0);

		glUseProgram(0);
		// prosses input queue event
		glfwPollEvents();
		// render frame butfer
		glfwSwapBuffers(Window);
	}

	glfwTerminate();
	return 0;
}
