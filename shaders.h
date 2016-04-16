#pragma once

#include "gl_core_4_3.h"

namespace Shaders
{
	constexpr GLchar simpleVS[] =
		R"(
	#version 430

	layout (location=0) in vec3 VertexPosition;
		
	uniform mat4 trans;	
	
	void main()
	{ 
		gl_Position = trans * vec4(VertexPosition, 1.0);
	}
)";

	constexpr GLchar simpleFS[] =
		R"(
	#version 430
	
	layout (location=0) out vec4 FragColor;

	void main()
	{
		FragColor=vec4(1.0);
	}
		
)";

	constexpr GLchar diffuseLightVS[] =
		R"(
	#version 430
	layout (location=0) in vec3 VertexPosition;
	layout (location=1) in vec3 VertexNormal;
	
	out vec3 lightIntensity;

	uniform mat4 trans;
	uniform vec4 lightPosition;
 
	void main()
	{
		vec3 Kd = vec3(1.0);
		vec3 Ld = vec3(0.75);	
		
		vec3 tnorm = normalize( (trans * vec4(VertexNormal, 0.0)).xyz );
		vec4 eyeCoord = trans * vec4(VertexPosition, 1.0);
		vec3 s = normalize(vec3(lightPosition - eyeCoord));
		
		lightIntensity  = Ld * Kd * max( dot(s, tnorm), 0.0);

		gl_Position = eyeCoord;		
	}	
	
)";

	constexpr GLchar diffuseLightFS[] =
		R"(
	#version 430

	layout (location=0) out vec4 FragColor;

	in vec3 lightIntensity; 

	void main()
	{
		FragColor=vec4(lightIntensity , 1.0);
	}
		
)";
}

