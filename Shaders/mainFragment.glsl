#version 430
	
	//layout(early_fragment_tests) in;

	// ======== START LIMITS =======

	const int lightMaxNumber = 5;
	const int maxMixTexturesNumber = 5;

	// ======== END LIMITS =========

	//======== STRUCTS START =======

	struct LightSource
	{
		int type;
		vec4 position;
		vec3 direction;
		vec3 intensityAmbient;
		vec3 intensityDiffuse;
		vec3 intensitySpecular;
		float shiness;
		float coneAngle;
		float coneShiness;
	};

	struct Settings
	{
		int cartoonLevelsNumber;
		float minFogDistance;
		float maxFogDistance;

		// CAST TO BOOL
		int isCartoon;
		int isFog;
	};

	struct MixTexture
	{
		sampler2D texture;
		float mixWeight;
	};

	struct TransparentTexture
	{
		sampler2D texture;
		float transparentEdge;
	};

	struct Material
	{
		vec3 Kd;
		vec3 Ka;
		vec3 Ks;
		int mixTexturesNumber;
		// Textures samplers
		sampler2D diffuseTexture;
		sampler2D normalTexture;
		TransparentTexture transparentTexture;
		MixTexture mixTextures[maxMixTexturesNumber];
	};

	//======= STRUCTS END =========

	//======= UNIFORM DATA START ========

	subroutine vec3 shadeModelType (LightSource lightSources[lightMaxNumber], vec4 position, vec3 normal);
	subroutine vec3 toObjectLocalCoordType (vec3 vec);
	subroutine vec3 getNormalVecType(vec3 originalNormal);
	subroutine uniform shadeModelType shadeModel;
	subroutine uniform toObjectLocalCoordType toObjectLocalCoord;
	subroutine uniform getNormalVecType getNormalVec;
	
	layout (location=0) out vec4 FragColor;

	uniform Settings settings;
	uniform LightSource lightSources[lightMaxNumber];
	uniform int lightSourcesNumber;
	uniform int useEffects;
	uniform Material material;
	

	in vec4 position;
    in vec3 normal;
	in vec2 textureCoord;
	in mat3 toObjectLocal;

	//======== UNIFORM DATA END ========
	
	// ======== SUBROUTINES START =========

	
	// This subroutine is for transformation to local object coordinate system
	subroutine (toObjectLocalCoordType)
	vec3 TransformToObjectLocal(vec3 vec)
	{
		return vec * toObjectLocal;
	}

	subroutine (toObjectLocalCoordType)
	vec3 NotTransformToObjectLocal(vec3 vec)
	{
		return vec;
	}
	// ======== SUBROUTINES END =========

	// ====== LIGHTS START ======
	vec3 PointLight (LightSource lightSource, vec4 position, vec3 normal, out vec3 spec)
	{
		vec3 s = normalize(toObjectLocalCoord(vec3(lightSource.position - position)));
		// DEBUG swap
		//vec3 v = normalize(toObjectLocalCoord(-position.xyz));
		vec3 v = normalize(toObjectLocalCoord(vec3(0.0, 0.0, -1.0) - position.xyz));
		vec3 r = reflect( -s, normal);
	
		// Half path vector optimization
        //vec3 h = normalize(v + s);
		vec3 ambient = lightSource.intensityAmbient * material.Ka;
		float sDotN = max( dot(s, normal), 0.0);
		vec3 diffuse = vec3(0.0);
		spec = vec3(0.0);
		if (sDotN > 0.0)
		{
			diffuse = lightSource.intensityDiffuse * material.Kd * sDotN;
			spec = lightSource.intensitySpecular * material.Ks * pow(max(dot(r,v) ,0.0), lightSource.shiness);
		}
		return ambient + diffuse;
	}

	vec3 DirectionLight (LightSource lightSource, vec4 position, vec3 normal, out vec3 spec)
	{
		vec3 s = normalize(toObjectLocalCoord(lightSource.direction));
		// Half path vector optimization
		//vec3 h = normalize(v + s);
		vec3 ambient = lightSource.intensityAmbient * material.Ka;
		float sDotN = max( dot(s, normal), 0.0);
		vec3 diffuse = vec3(0.0);
		spec = vec3(0.0);
		if (sDotN > 0.0)
		{
			vec3 v = normalize(toObjectLocalCoord(-position.xyz));
			vec3 r = reflect(-s, normal);
			diffuse = lightSource.intensityDiffuse * material.Kd * sDotN;
			spec = lightSource.intensitySpecular * material.Ks * pow(max(dot(r,v) ,0.0), lightSource.shiness);
		}
		return ambient + diffuse;	
	}
	
	vec3 ConeLight(LightSource lightSource, vec4 position, vec3 normal, out vec3 spec)
	{
		vec3 s = normalize(toObjectLocalCoord(vec3(lightSource.position - position)));
		float angle = acos(dot(-s, lightSource.direction));
		float cutoff = radians( clamp(lightSource.coneAngle, 0.0, 90.0));
		vec3 ambient = lightSource.intensityAmbient * material.Ka;
		if (angle <= cutoff)
		{
			// Half path vector optimization
			//vec3 h = normalize(v + s);
			float sDotN = max( dot(s, normal), 0.0);
			float spotFactor = pow( dot(-s, lightSource.direction), lightSource.coneShiness);
			vec3 v = normalize(toObjectLocalCoord(-position.xyz));
			vec3 r = reflect(-s, normal);
			vec3 diffuse = lightSource.intensityDiffuse * material.Kd * sDotN;
			spec = lightSource.intensitySpecular * material.Ks * pow(max(dot(r,v) ,0.0), lightSource.shiness);
			spec *= spotFactor;
			return ambient + spotFactor * diffuse;
		}
		else
		{
			spec = vec3(0.0);
			return ambient;
		}
	}

	// ======== LIGHTS END =======

	// ======== EFFECTS START=====

	vec3 CartoonEffect(vec3 initColor )
	{
		float oneStep = 1.0 / settings.cartoonLevelsNumber;
		initColor = floor( initColor / oneStep);
		initColor = initColor * oneStep;

		return initColor;
	}

	vec3 FogEffect(vec3 initColor, vec4 position)
	{
		float fogFactor = (settings.maxFogDistance - position.z) / (settings.maxFogDistance - settings.minFogDistance);	
		fogFactor = clamp(fogFactor, 0.0, 1.0) ;
		initColor = mix(vec3(0.0, 0.0, 0.0), initColor, fogFactor);

		return initColor;
	}

	vec3 UseEffects(vec3 initColor, vec4 position)
	{
		if (bool(settings.isCartoon))
			initColor = CartoonEffect( initColor );
		if (bool(settings.isFog))
			initColor = FogEffect(initColor, position);

		return initColor;
	}

	// ========= EFFECTS END ==========
	// ========= TEXTURES START =======

	void ApplyTransparentTexture()
	{
		vec4 texColor = texture(material.transparentTexture.texture, textureCoord);
		if (texColor.y < material.transparentTexture.transparentEdge)
		{
			discard;
		}
	}

	vec4 ApplyMixTextures(vec4 texColor)
	{
		for (int i = 0 ; i < material.mixTexturesNumber; ++i)
		{
			vec4 mixTexColor = texture(material.mixTextures[i].texture, textureCoord);
			texColor = mix(texColor, mixTexColor, material.mixTextures[i].mixWeight);
		}

		return texColor;
	}

	vec4 ApplyTextures()
	{
		// Apply transparent texture
		ApplyTransparentTexture();
		// Apply diffuse texture
		vec4 texColor = texture(material.diffuseTexture, textureCoord);
		// Apply mix textures
		texColor = ApplyMixTextures(texColor);
		return texColor;
	}
	// ======== TEXTURES END =========

	// ======== SUBROUTINES START =========


	subroutine ( shadeModelType )
	vec3 PhongLight(LightSource lightSources[lightMaxNumber], vec4 position, vec3 normal)
	{
		vec3 lightIntensity = vec3(0.0);
		vec3 specComp = vec3(0.0);
		vec3 spec = vec3(0.0);
		vec3 normalVec = getNormalVec(normal);
		for (int i=0; i < lightSourcesNumber; ++i)
		{
			switch(lightSources[i].type)
			{
			case 0:
               lightIntensity += vec3(0.0, 0.0, 0.0);
			   break;
            case 1:
               lightIntensity += PointLight(lightSources[i], position, normalVec, spec);
			   specComp += spec;
			   break;
            case 2:
			   lightIntensity += DirectionLight(lightSources[i], position, normalVec, spec);
			   specComp += spec;
			   break;
			case 3:
			   lightIntensity += ConeLight(lightSources[i], position, normalVec, spec);
			   specComp += spec;
			   break;
			}
		}
		vec4 texColor = ApplyTextures();
		lightIntensity = lightIntensity * vec3(texColor.x, texColor.y, texColor.z);
		lightIntensity = UseEffects(lightIntensity, position) + specComp;
		return lightIntensity;
	}

	subroutine (shadeModelType)
	vec3 LighSourceLight(LightSource lightSources[lightMaxNumber], vec4 position, vec3 normal)
	{
		return vec3(0.2, 1.0, 0.2);
	}

	// This subroutine if for getting right normal vector. From texture or not
	subroutine (getNormalVecType)
	vec3 GetNormalFromTexture(vec3 originalNormal)
	{
		vec4 texNormal = 2.0 * texture( material.normalTexture, textureCoord ) - 1.0;
		return normalize(texNormal.xyz);
	}

	subroutine (getNormalVecType)
	vec3 GetOriginalNormal(vec3 originalNormal)
	{	
		return originalNormal;
	}

	// ======== SUBROUTINES END =========

	void main()
	{	

		vec3 lightIntensity;
		if (!gl_FrontFacing)
			lightIntensity = shadeModel(lightSources, position, normal);
		else
			lightIntensity = shadeModel(lightSources, position, -normal);
		FragColor=vec4(lightIntensity , 1.0);
	}