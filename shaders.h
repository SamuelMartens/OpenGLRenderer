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

	constexpr GLchar phongLightVS[] =
		R"(
	#version 430
	layout (location=0) in vec3 VertexPosition;
	layout (location=1) in vec3 VertexNormal;
	
	flat out vec3 lightIntensity;

									uniform mat4 trans;
	uniform vec4 lightPosition;
 
	void main()
	{
		vec3 Kd = vec3(1.0);
		vec3 Ka = vec3(1.0);
		vec3 Ks = vec3(1.0);
		vec3 Ld = vec3(0.5);
		vec3 La = vec3(0.2);
		vec3 Ls = vec3(0.9);
		float shiness = 10;
		
		vec3 tnorm = normalize((trans * vec4(VertexNormal, 0.0)).xyz);
		vec4 eyeCoord = trans * vec4(VertexPosition, 1.0);
		vec3 s = normalize(vec3(lightPosition - eyeCoord));
		vec3 v = normalize(-eyeCoord.xyz);
		vec3 r = reflect(-s, tnorm);
		vec3 ambient = La * Ka;
		float sDotN = max( dot(s, tnorm), 0.0);
		vec3 diffuse = Ld * Kd * sDotN;
		vec3 spec = vec3(0.0);
		if (sDotN > 0.0)
			spec = Ls * Ks * pow(max(dot(r,v) ,0.0), shiness);

		lightIntensity = ambient + diffuse + spec;
		gl_Position = eyeCoord;		
	}	
	
)";

	constexpr GLchar phongLightFS[] =
		R"(
	#version 430

	layout (location=0) out vec4 FragColor;

	flat in vec3 lightIntensity; 

	void main()
	{
		FragColor=vec4(lightIntensity , 1.0);
	}
		
)";
}

