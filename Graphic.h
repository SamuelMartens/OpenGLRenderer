#pragma once


#include <vector>
#include "gl_core_4_3.h"
#include "glm\mat4x4.hpp"

namespace Graphic 
{
	void InitFigure(std::vector<float>& vertices, std::vector<float>& colors);

	enum class VertexAtrib
	{
		VertexCoors,
		Colors
	};

	class Renderer
	{
	public:
		Renderer();
		~Renderer();
		int Init();
		int InitShaders();
		int InitUniforms();
		void Draw();
		void Reload();
		void SetTransMatrix(glm::mat4 &transMat);

		std::vector<float> vertices;
		std::vector<float> colors;
	private:
		GLuint vertexArrayBuffer;
		GLuint verticesBuffer;
		GLuint colorBuffer;

		GLuint shaderProgram;
		GLuint vertShader;
		GLuint fragShader;

		/* Matrix */
		GLint transMat;
	};
}