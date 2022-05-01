#include "../headers/RenderInfo.h"
#include "../headers/includes.h"

void RenderInfo::ShowInfo()
{
	GLint GLMajorVersion = 0;
	GLint GLMinorVersion = 0;

	glGetIntegerv(GL_MAJOR_VERSION, &GLMajorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &GLMinorVersion);
	std::cout << "OpenGL Version: " << GLMajorVersion << "." << GLMinorVersion << std::endl;
	std::cout << "Vendor        :" << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer      :" << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Full Version  :" << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL          :" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}
