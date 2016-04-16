#pragma once

#include <vector>

#include "gl_core_4_3.h"
#include "glm\mat4x4.hpp"
#include "glm\vec4.hpp"

#include "model.h"

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
		Renderer();
		~Renderer();
		int Init();
		int InitShaders();
		int InitUniforms();

		void Draw(float angle);
		void Reload(float angle); // Do nothing
		void ClearScreen() const;

		void SetTransMatrix(glm::mat4 &transMat);
		void SetLightPostion(float x, float y, float z);

		void AddModel(const Model&& m);

	private:
		std::vector<Model> models;
		glm::vec4 lightPosition;

		/* Buffers */
		GLuint shaderProgram;
		GLuint vertShader;
		GLuint fragShader;

		/* Uniforms */ 
		GLint transMatLoc;
		GLint lightPosLoc;
	};
}