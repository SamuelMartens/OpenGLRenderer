#include "InitProgram.h"
#include "Graphic.h"
#include "model.h"
#include "Settings.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "DiffuseTexture.h"
#include "MixTexture.h"
#include "TransparentTexture.h"
#include "NormalTexture.h"
#include "Camera.h"
#include "Viewport.h"
#include "Utils.h"

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

	ShaderProgram shaderProgram(vertexShader, fragmentShader, ShaderProgram::Type::Main);

	/* Init renderer */
	//renderer.AddShaderProgram(new ShaderProgram(vertexShader, fragmentShader, ShaderProgram::Type::Main));
	Graphic::Renderer::Instance().AddShaderProgram(&shaderProgram);

	if (0 != Graphic::Renderer::Instance().Init())
		std::cout << "Failed to init renderer";
	/* Renderer camera and viewport settings */
	Graphic::Renderer::Instance().SetCamera(std::make_unique<Camera>(
		  glm::vec3(0, 0, -2)
		, glm::vec3(0, 0, 0)
		, std::make_unique<Viewport>()));

	std::unique_ptr<Viewport>& rendererVp = Graphic::Renderer::Instance().GetCamera()->GetViewport();
	rendererVp->SetNearFar(0.01, 100);
	rendererVp->SetAspect(1);
	rendererVp->SetFov(Utils::DegToRad(90));

	/* Settings change */


	/* Add models */
	Model model;
	Material material;
	material.SetTexture(std::make_shared<DiffuseTexture>());
	//material.GetTextureWithType(Texture::Type::Diffuse)->Load("E:\\C++\\OpenGLtutorial\\texture.jpg", *renderer.GetShaderProgramWithType(ShaderProgram::Type::Main));
	material.GetTextureWithType(Texture::Type::Diffuse)->Load("E:\\C++\\OpenGLtutorial\\Stormtrooper\\Stormtrooper_D.tga", *Graphic::Renderer::Instance().GetShaderProgramWithType(ShaderProgram::Type::Main));
	//material.SetTexture(std::make_shared<TransparentTexture>(0.9));
	//material.GetTextureWithType(Texture::Type::Transparent)->Load("E:\\C++\\OpenGLtutorial\\texture.jpg", shaderProgram);
	material.SetTexture(std::make_shared<NormalTexture>());
	//material.GetTextureWithType(Texture::Type::Normal)->Load("E:\\C++\\OpenGLtutorial\\texture.jpg", *renderer.GetShaderProgramWithType(ShaderProgram::Type::Main));
	material.GetTextureWithType(Texture::Type::Normal)->Load("E:\\C++\\OpenGLtutorial\\Stormtrooper\\Stormtrooper_N.tga", *Graphic::Renderer::Instance().GetShaderProgramWithType(ShaderProgram::Type::Main));
	model.material = material;
	model.LoadModel("E:\\C++\\OpenGLtutorial\\Stormtrooper\\Stormtrooper.obj");
	//model.LoadModel("E:\\C++\\OpenGLtutorial\\resources\\sphere.obj");
	//model.scale = 0.5;
	//model.position = glm::vec4(0.5, 0.5, 0, 1);
	Graphic::Renderer::Instance().AddModel(model);

	/* Add light */
	Light l1, l2;
	l1.SetDirection(glm::vec3(0, 0, 5));
	l1.SetPosition(glm::vec4(0, 2, 0.0, 1));
	l1.coneAngle = 50;
	l1.coneShiness = 50;
	l1.type = Light::LighType::PointLight;
	Graphic::Renderer::Instance().AddLight(l1);

	long float angleY = 0;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{

		/* Clear screen */
		Graphic::Renderer::Instance().ClearScreen();

		/* Render here */
		Graphic::Renderer::Instance().Draw(angleY);

		/* Change data here */
		angleY += 0.002;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		glfwPollEvents();

	}

	glfwTerminate();
	return 0;
}