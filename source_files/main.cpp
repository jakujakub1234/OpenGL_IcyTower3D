#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "../header_files/GLCore.h"

int main()
{
	GLCore window = GLCore();

	window.RenderLoop();
}