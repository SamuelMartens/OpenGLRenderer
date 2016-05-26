	#version 430
	layout (location=0) in vec3 VertexPosition;
	layout (location=1) in vec3 VertexNormal;
	
	out vec3 normal;
	out vec4 position;	

	uniform mat4 trans;
 
	void main()
	{
		
		normal = normalize((trans * vec4(VertexNormal, 0.0)).xyz);
		position = trans * vec4(VertexPosition, 1.0);

		gl_Position = position;		
	}	