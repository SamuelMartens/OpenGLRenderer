#include "InitProgram.h"
#include "Graphic.h"
#include "model.h"

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

	window = glfwCreateWindow(600, 600, "Hello world", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (-1 == InitProgram::InitProgram())
		return 1;

	InitProgram::SetDebugCallback();

	/* Create renderer */
	Graphic::Renderer renderer;
	
	if (0 != renderer.Init())
	{
		std::cout << "Failed to init renderer";
		return 1;
	};

	/* Add models */
	Model model;
	model.LoadModel("E:\\C++\\OpenGLtutorial\\Stormtrooper.obj");
	renderer.AddModel(std::move(model));

	float angleY = 0;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{

		/* Clear screen */
		renderer.ClearScreen();

		/* Render here */
		renderer.Draw(angleY);

		/* Change data here */
		angleY += 0.0005;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		glfwPollEvents();

	}

	glfwTerminate();
	return 0;
}