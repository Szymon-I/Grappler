#version 150

#define LIGHT_ATTENUATION 0.01
#define LIGHT_SPECULAR    vec3(2.0, 2.0, 2.0)

#define MAX_LIGHTS        20

in vec4 ourPosLight;

// Potrzebne do shadowMap i oswietlenia kierunkowego
uniform sampler2D shadowMap;
uniform vec3 Light_Direction;

struct LightParameters
{
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
};

in vec4 ourPosition;
in vec3 ourNormal;
in vec2 inoutUV;

out vec4 outColor;

uniform int Number_Of_Lights;
uniform vec3 Light_Position[MAX_LIGHTS];
uniform vec3 Light_Ambient[MAX_LIGHTS];
uniform vec3 Light_Diffuse[MAX_LIGHTS];

struct Material
{
	vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    float Shininess;
};
uniform Material myMaterial;

uniform vec3 Camera_Position;

uniform sampler2D tex0;

// ---------------------------------------------------------------------------
vec3 Calculate_DirectionalLight(vec3 lightDiffuse, vec3 lightSpecular, vec3 LightDirection)
{
	// diffuse
	vec3  lightDirection = normalize(LightDirection);
	float diffuseCoeff = max(dot(ourNormal, -lightDirection), 0.0);
	vec3  resultDiffuse = diffuseCoeff * vec3(lightDiffuse);
	vec3  diffusePart = resultDiffuse;

	// specular
	vec3  viewDir = normalize(Camera_Position - vec3(ourPosition));
	vec3  reflectDir = reflect(lightDirection, ourNormal);
	float specularCoeff = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3  specularPart = specularCoeff * vec3(lightSpecular);

	// result
	return (diffusePart + specularPart);
}

float Calculate_Shadow(void)
{

	// perform perspective divide
    vec3 projCoords = ourPosLight.xyz / ourPosLight.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    if(projCoords.z > 1.0)
        return 0.0;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

	// calculate bias according to the angle between normal vector and light vector
    float bias = max(0.004 * (1.0 - dot(ourNormal, Light_Direction)), 0.001);

	// Smooth filtering
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(float x = -1; x <= 1; x+=1.0)
	{
		for(float y = -1; y <= 1; y+=1.0)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;

	return shadow;
}

// ---------------------------------------------------------------------------
void main()
{

	// Zastosowanie oswietlenia do fragmentu
	vec4 objectColor = texture( tex0, inoutUV );

	if(objectColor.a < 0.1)
    	discard;

	// zmienna kumulujaca swiatlo punktowe
	vec3 pointLights = vec3(0.0);
    vec3 ambients = vec3(0.0);
	vec3 specularPart = vec3(0.0);

	for (int i=0; i<Number_Of_Lights; i++)
	{
		//zmniejszenie mocy wraz z odlegloscia
	    float odl = distance(vec3(ourPosition),Light_Position[i]);
		float odlMod = 1.0 / (1.0 + LIGHT_ATTENUATION * (odl * odl));

		// Obliczenie wektora (swiatlo - wierzcholek)
		// czyli kierunku padania swiatla na wierzcholek
		vec3 lightDirection = normalize(Light_Position[i] - vec3(ourPosition));

		// obliczenie kata pomiedzy wektorem lightDir oraz wektorem normalnym
		// wartosc kata okresla pod jakim katem padaja promienie
		float lightCoeff = max(dot(ourNormal, lightDirection), 0.0);

		// obliczenie skladowej diffuse
		pointLights += lightCoeff * Light_Diffuse[i] * odlMod;

        ambients += Light_Ambient[i];

		// Specular
		vec3 viewDir = normalize(Camera_Position - vec3(ourPosition));
		vec3  reflectDir = reflect(-lightDirection, ourNormal);
		float specularCoeff = pow(max(dot(viewDir, reflectDir), 0.0), myMaterial.Shininess);
		vec3  resultSpecular = specularCoeff * LIGHT_SPECULAR * myMaterial.Specular;

		specularPart += resultSpecular * odlMod;

	}

	vec4 result =  (vec4(ambients/Number_Of_Lights, 1.0) + vec4(pointLights, 1.0)+ vec4(specularPart, 1.0)) * objectColor;

	LightParameters myLight;

	// Parametry swiatla
	myLight.Ambient = vec3(0.4, 0.4, 0.4);
	myLight.Diffuse = vec3(0.8, 0.8, 0.8);
	myLight.Specular = vec3(0.5, 0.5, 0.5);

	// Kolor wyjsciowy
	vec3  myColor = vec3(result.xyz);

	// Skladowe
	vec3  lightPart = Calculate_DirectionalLight(myLight.Diffuse, myLight.Specular, Light_Direction);

	// Kolor finalny
	vec3  finalColor = (myLight.Ambient + lightPart) * myColor - Calculate_Shadow();

	outColor = vec4(finalColor , result.w);

}
