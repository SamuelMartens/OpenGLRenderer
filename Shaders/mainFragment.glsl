#version 430

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

	subroutine vec3 shadeModelType (LightSource lightSource , vec4 position, vec3 normal);
	subroutine uniform shadeModelType shadeModel;
	
	layout (location=0) out vec4 FragColor;

	uniform LightSource lightSources[5];
	uniform int lightSourcesNumber;

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

	subroutine ( shadeModelType )
	vec3 PhongLight(LightSource lightSource, vec4 position, vec3 normal)
	{
		switch(lightSource.type)
        {
			case 0:
               return vec3(0.0, 0.0, 0.0);
            case 1:
               return PointLight(lightSource, position, normal);
            case 2:
			   return DirectionLight(lightSource, position, normal);
			case 3:
			   return ConeLight(lightSource, position, normal);
		}
	}
	
	subroutine (shadeModelType)
	vec3 LighSourceLight(LightSource lightSource, vec4 position, vec3 normal)
	{
		return vec3(0.2, 1.0, 0.2);
	}

	void main()
	{
		vec3 lightIntensity = vec3(0);
		for (int i=0; i < lightSourcesNumber; ++i)
			lightIntensity += shadeModel(lightSources[i], position, normal);
		FragColor=vec4(lightIntensity , 1.0);
	}