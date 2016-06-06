#include "InitProgram.h"
#include "Graphic.h"
#include "model.h"
#include "Settings.h"
#include "ShaderProgram.h"
#include "Shader.h"

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
	
	/* Creating of shaders and shader program */
	Shader vertexShader(Shader::Type::Vertex, "E:\\C++\\OpenGLtutorial\\Shaders\\mainVertex.glsl");
	Shader fragmentShader(Shader::Type::Fragment, "E:\\C++\\OpenGLtutorial\\Shaders\\mainFragment.glsl");
	ShaderProgram shaderProgram(vertexShader, fragmentShader);

	/* Set settings */
	Settings settings;
	settings.cartoonLevels = 4;
	settings.cartoon = false;

	/* Create and init renderer */
	Graphic::Renderer renderer(settings);
	renderer.shaderProgram = shaderProgram;
	
	if (0 != renderer.Init())
		std::cout << "Failed to init renderer";

	/* Add models */
	Model model;
	model.LoadModel("E:\\C++\\OpenGLtutorial\\Stormtrooper.obj");
	renderer.AddModel(model);

	/* Add light */
	Light l1, l2;
	l1.SetDirection(glm::vec3(0, 0, 5));
	l1.SetPosition(glm::vec4(0, 0, -1, 1));
	l1.coneAngle = 50;
	l1.coneShiness = 50;
	l1.type = Light::LighType::ConeLigh;
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