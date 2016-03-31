#include "InitProgram.h"
#include "Graphic.h"

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

	window = glfwCreateWindow(640, 480, "Hello world", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (-1 == InitProgram::InitProgram())
		return 1;

	Graphic::Renderer renderer;
	Graphic::InitFigure(renderer.vertices, renderer.colors);
	if (0 != renderer.Init())
	{
		std::cout << "Failed to init renderer";
		return 1;
	};

	float angle = 0.0001;

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