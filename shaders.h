#pragma once

#include "gl_core_4_3.h"

namespace Shaders
{
	constexpr GLchar vertextShader[] =
		R"(
	#version 430

	layout (location=0) in vec3 VertexPosition;
		
	uniform mat4 trans;	
	
	void main()
	{
		vec4 newPos =  trans * vec4(VertexPosition, 1.0);
		gl_Position = newPos;
	}
)";

	constexpr GLchar fragmentShader[] =
		R"(
	#version 430
	
	layout (location=0) out vec4 FragColor;

	void main()
	{
		FragColor=vec4(1.0);
	}
		
)";
}

