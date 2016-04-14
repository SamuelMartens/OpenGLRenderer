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
		void Draw() const;
		void Reload(float angle);
		void ClearScreen() const;
		void SetTransMatrix(glm::mat4 &transMat);

	private:

		GLuint shaderProgram;
		GLuint vertShader;
		GLuint fragShader;

		GLint transMatLoc;
	};
}