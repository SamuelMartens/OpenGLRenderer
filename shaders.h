#pragma once

#include "gl_core_4_3.h"

namespace Shaders
{
	constexpr GLchar vertextShader[] =
		R"(
	#version 430

			layout (location=0) in vec3 VertexPosition;
	layout (location=1) in vec3 VertexColor;
		
	uniform mat3 trans;	

			out vec3 Color;
	
	void main()
	{
		
		Color = VertexColor;
		vec3 newPos = trans * VertexPosition;
		gl_Position = vec4(newPos, 1.0);
	}
)";

	constexpr GLchar fragmentShader[] =
		R"(
	#version 430
	
	in vec3 Color;
	
	layout (location=0) out vec4 FragColor;

	void main()
	{
		FragColor=vec4(Color, 1.0);
	}
		
)";
}

