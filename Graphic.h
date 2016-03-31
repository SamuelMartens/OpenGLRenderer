#pragma once


#include <vector>
#include "gl_core_4_3.h"


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
		void Draw();
		void Reload();

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