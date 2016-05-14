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

	/* Add models */
	Model model;
	model.LoadModel("E:\\C++\\OpenGLtutorial\\Stormtrooper.obj");
	renderer.AddModel(model);

	/* Add light */
	Light l1, l2;
	l1.direction = glm::vec3(0, -0.25, 1);
	l1.SetPosition(glm::vec4(0, 0.5, -1, 1));
	l1.coneAngle = 15;
	l1.coneShiness = 150;
	l1.type = Light::LighType::ConeLigh;
	l2.SetPosition(glm::vec4(-0.7, -0.2, 0.2, 1));
	l2.type = Light::LighType::PointLight;
	//renderer.AddLight(l2);
	renderer.AddLight(l1);

	

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