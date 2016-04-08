#include "InitProgram.h"
#include "Graphic.h"
#include "ext_glm.h"

#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>


int main()
{
	GLFWwindow *window;

	/* Init the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context   */

	// Enable debug
	//glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	//glEnable(GL_DEBUG_OUTPUT);

	window = glfwCreateWindow(640, 480, "Hello world", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (-1 == InitProgram::InitProgram())
		return 1;

	InitProgram::SetDebugCallback();

	Graphic::Renderer renderer;
	Graphic::InitFigure(renderer.vertices, renderer.colors);
	if (0 != renderer.Init())
	{
		std::cout << "Failed to init renderer";
		return 1;
	};

	float angle = 2;

	renderer.SetTransMatrix(ext_glm::rotateZ(angle));

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		//renderer.Reload();
		renderer.Draw();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		glfwPollEvents();

	}

	glfwTerminate();
	return 0;
}