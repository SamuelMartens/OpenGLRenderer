#version 430
	
	const int lightMaxNumber = 5;

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

	subroutine vec3 shadeModelType (LightSource lightSources[lightMaxNumber], vec4 position, vec3 normal);
	subroutine uniform shadeModelType shadeModel;
	
	layout (location=0) out vec4 FragColor;

	uniform LightSource lightSources[lightMaxNumber];
	uniform int lightSourcesNumber;
	uniform Settings settings;
	uniform int useEffects;

	uniform vec3 Kd;
	uniform vec3 Ka;
	uniform vec3 Ks;
	
	in vec4 position;
    in vec3 normal;

	vec3 PointLight (LightSource lightSource, vec4 position, vec3 normal)
	{
		vec3 s = normalize(vec3(lightSource.position - position));
		vec3 v = normalize(-position.xyz);
		vec3 r = reflect(-s, normal);
        // Half path vector optimization
        //vec3 h = normalize(v + s);
		vec3 ambient = lightSource.intensityAmbient * Ka;
		float sDotN = max( dot(s, normal), 0.0);
		vec3 diffuse = vec3(0.0);
		vec3 spec = vec3(0.0);
		if (sDotN > 0.0)
		{
			diffuse = lightSource.intensityDiffuse * Kd * sDotN;
			spec = lightSource.intensitySpecular * Ks * pow(max(dot(r,v) ,0.0), lightSource.shiness);
		}
		return ambient + diffuse + spec;
	}

	vec3 DirectionLight (LightSource lightSource, vec4 position, vec3 normal)
	{
		vec3 s = normalize(lightSource.direction);
		// Half path vector optimization
		//vec3 h = normalize(v + s);
		vec3 ambient = lightSource.intensityAmbient * Ka;
		float sDotN = max( dot(s, normal), 0.0);
		vec3 diffuse = vec3(0.0);
		vec3 spec = vec3(0.0);
		if (sDotN > 0.0)
		{
			vec3 v = normalize(-position.xyz);
			vec3 r = reflect(-s, normal);
			diffuse = lightSource.intensityDiffuse * Kd * sDotN;
			spec = lightSource.intensitySpecular * Ks * pow(max(dot(r,v) ,0.0), lightSource.shiness);
		}
		return ambient + diffuse + spec;	
	}
	
	vec3 ConeLight(LightSource lightSource, vec4 position, vec3 normal)
	{
		vec3 s = normalize(vec3(lightSource.position - position));
		float angle = acos(dot(-s, lightSource.direction));
		float cutoff = radians( clamp(lightSource.coneAngle, 0.0, 90.0));
		vec3 ambient = lightSource.intensityAmbient * Ka;
		if (angle <= cutoff)
		{
			// Half path vector optimization
			//vec3 h = normalize(v + s);
			float sDotN = max( dot(s, normal), 0.0);
			float spotFactor = pow( dot(-s, lightSource.direction), lightSource.coneShiness);
			vec3 v = normalize(-position.xyz);
			vec3 r = reflect(-s, normal);
			vec3 diffuse = lightSource.intensityDiffuse * Kd * sDotN;
			vec3 spec = lightSource.intensitySpecular * Ks * pow(max(dot(r,v) ,0.0), lightSource.shiness);
			return ambient + spotFactor * ( diffuse + spec );
		}
		else
		{
			return ambient;
		}
	}

	vec3 CartoonEffect(vec3 initColor )
	{
		float oneStep = 1.0 / settings.cartoonLevelsNumber;
		initColor = floor( initColor / oneStep);
		initColor = initColor * oneStep;

		return initColor;
	}

	vec3 FogEffect(vec3 initColor, vec4 position)
	{
		float dist = abs(position.z);
		float fogFactor = (settings.maxFogDistance - dist) / (settings.maxFogDistance - settings.minFogDistance);	
		fogFactor = clamp(fogFactor, 0.0, 1.0);
		initColor = mix(vec3(0.0, 0.0, 0.0), initColor, fogFactor);
		// DEBUG
		// initColor = vec3(0, 1, 0);
		// END

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

	subroutine ( shadeModelType )
	vec3 PhongLight(LightSource lightSources[lightMaxNumber], vec4 position, vec3 normal)
	{
		vec3 lightIntensity = vec3(0.0);
		for (int i=0; i < lightSourcesNumber; ++i)
		{
			switch(lightSources[i].type)
			{
			case 0:
               lightIntensity += vec3(0.0, 0.0, 0.0);
            case 1:
               lightIntensity += PointLight(lightSources[i], position, normal);
            case 2:
			   lightIntensity += DirectionLight(lightSources[i], position, normal);
			case 3:
			   lightIntensity += ConeLight(lightSources[i], position, normal);
			}
		}

		lightIntensity = UseEffects(lightIntensity, position);
		return lightIntensity;
	}
	
	subroutine (shadeModelType)
	vec3 LighSourceLight(LightSource lightSources[lightMaxNumber], vec4 position, vec3 normal)
	{
		return vec3(0.2, 1.0, 0.2);
	}

	void main()
	{
		vec3 lightIntensity = shadeModel(lightSources, position, normal);
		FragColor=vec4(lightIntensity , 1.0);
	}