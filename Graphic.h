#pragma once

#include <vector>
#include <memory>

#include "gl_core_4_3.h"
#include "glm\mat4x4.hpp"
#include "glm\vec4.hpp"

#include "Light.h"
#include "model.h"
#include "Settings.h"

namespace Graphic 
{
	void InitFigure(std::vector<float>& vertices, std::vector<float>& colors);

	enum class VertexAtrib
	{
		VertexCoors,
		Normals
	};

	class Renderer
	{
	public:
		Renderer(Settings& set);
		~Renderer();
		/* Initializing */
		int Init();
		int InitShaders();
		int InitUniforms();

		/* Drawing */
		void Draw(float angle);
		void Reload(float angle); // Do nothing
		void ClearScreen() const;

		/* Set OpenGL positions */
		void SetTransMatrix(glm::mat4 &transMat);
		void LoadLightDataToOpenGL() const;

		/* Adding of elements */
		void AddModel(const Model&& m);
		void AddModel(const Model& m);
		void AddLight(const Light&& l);
		void AddLight(const Light& l);
		void AddLight(std::unique_ptr<Light> l);

		Settings settings;
	private:
		std::vector<Model> models;
		std::vector<Light> lights;

		/* Buffers */
		GLuint shaderProgram;
		GLuint vertShader;
		GLuint fragShader;

		/* Uniforms */ 
		GLint transMatLoc;
		GLuint modelSubroutine;
		GLuint lightSubroutine;
	};
}