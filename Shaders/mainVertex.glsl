	#version 430
	layout (location=0) in vec3 VertexPosition;
	layout (location=1) in vec3 VertexNormal;
	layout (location=2) in vec2 TextureCoord;
	layout (location=3) in vec3 VertexTangent;
	layout (location=4) in vec3 VertexBitangent;

	struct MaterialVertex
	{
		// Cast this to bool
		int hasNormalTexture;
	};
	
	subroutine void drawObjectType();

	out vec3 normal;
	out vec4 position;	
	out vec2 textureCoord;
	out mat3 toObjectLocal;
	out vec3 reflectDir;

	subroutine uniform drawObjectType drawObject;

	uniform mat4 modelViewProjMat;
	uniform mat4 normalMat;
	uniform mat4 modelViewMat;
	
	uniform MaterialVertex materialVertex;

	subroutine (drawObjectType)
	void DrawModel()
	{
		textureCoord = TextureCoord;
		normal = normalize((normalMat * vec4(VertexNormal, 0.0)).xyz);

		vec3 tangent, bitangent;

		if (bool(materialVertex.hasNormalTexture))
		{
			tangent = normalize((normalMat * vec4(VertexTangent, 0.0)).xyz);
			bitangent = normalize((normalMat * vec4(VertexBitangent, 0.0)).xyz);

			toObjectLocal = mat3(
			tangent.x, bitangent.x, normal.x,
			tangent.y, bitangent.y, normal.y,
			tangent.z, bitangent.z, normal.z);
		}

		position = modelViewMat * vec4(VertexPosition, 1.0);
	}

	subroutine (drawObjectType)
	void DrawSkyBox()
	{
		reflectDir = VertexPosition;
	}

	void main()
	{
		
		drawObject();
		gl_Position = modelViewProjMat * vec4(VertexPosition, 1.0);		
	}	