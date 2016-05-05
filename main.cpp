#include "InitProgram.h"
#include "Graphic.h"
#include "model.h"
#include "Settings.h"

#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <memory>


int main()
{
	GLFWwindow *window;

	/* Init the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context   */

	window = glfwCreateWindow(600, 600, "Renderer", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (-1 == InitProgram::InitProgram())
		return 1;

	InitProgram::SetDebugCallback();

	/* Set settings */
	Settings settings;

	/* Create and init renderer */
	Graphic::Renderer renderer(settings);
	
	if (0 != renderer.Init())
		std::cout << "Failed to init renderer";

	Light l;
	l.SetPosition(glm::vec4(0.7, 0.2, 0.2, 1));
	l.type = Light::LighType::PointLight;
	
	renderer.AddLight(l);
	renderer.LoadLightDataToOpenGL();

	/* Add models */
	Model model;
	//model.LoadModel("E:\\C++\\OpenGLtutorial\\Stormtrooper.obj");
	// DEBUG
	model.LoadModel("E:\\C++\\OpenGLtutorial\\resources\\sphere.obj");
	model.scale = 0.25;
	// END
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