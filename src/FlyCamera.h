#include "includes.h"

class FlyCamera
{
public:
	// Matrix View
	glm::vec3 Location{0.0f, 0.0f, 5.0f};
	glm::vec3 Direction{0.0f, 0.0f, -1.0f};
	glm::vec3 Up{0.0f, 1.0f, 0.0f};
	// Matrix Projection
	float Fov = glm::radians(45.0f);
	float AspectRatio = 4 / 3;
	float Near = 0.001f;
	float Far = 1000.0f;

	FlyCamera();

	FlyCamera(int Width, int Height);

	FlyCamera(glm::vec3 location, glm::vec3 direction, glm::vec3 up,
			  float fov, float aspectRatio, float near, float far);

	FlyCamera(glm::vec3 location, glm::vec3 direction, glm::vec3 up);

	glm::mat4 GetViewProjection() const;
};