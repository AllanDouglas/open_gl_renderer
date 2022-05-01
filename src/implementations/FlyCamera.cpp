#include "../headers/FlyCamera.h"

FlyCamera::FlyCamera() {}

FlyCamera::FlyCamera(int Width, int Height)
{
	AspectRatio = Width / Height;
}

FlyCamera::FlyCamera(glm::vec3 location, glm::vec3 direction, glm::vec3 up,
					 float fov, float aspectRatio, float near, float far) : FlyCamera(location, direction, up)
{
	Fov = fov;
	AspectRatio = aspectRatio;
	Near = near;
	Far = far;
}

FlyCamera::FlyCamera(glm::vec3 location, glm::vec3 direction, glm::vec3 up)
{
	Location = location;
	Direction = direction;
	Up = up;
}

glm::mat4 FlyCamera::GetViewProjection() const
{
	auto ViewMatrix = glm::lookAt(Location, Location + Direction, Up);
	auto ProjectionMatrix = glm::perspective(Fov, AspectRatio, Near, Far);
	return ProjectionMatrix * ViewMatrix;
}