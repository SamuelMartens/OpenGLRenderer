	#version 430
	layout (location=0) in vec3 VertexPosition;
	layout (location=1) in vec3 VertexNormal;
	layout (location=2) in vec2 TextureCoord;
	
	out vec3 normal;
	out vec4 position;	
	out vec2 textureCoord;

	uniform mat4 trans;
 
	void main()
	{
		textureCoord = TextureCoord;
		normal = normalize((trans * vec4(VertexNormal, 0.0)).xyz);
		position = trans * vec4(VertexPosition, 1.0);

		gl_Position = position;		
	}	