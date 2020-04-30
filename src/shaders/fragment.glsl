#version 150 core

in vec4 ourPosition;
in vec3 ourNormal;
in vec2 inoutUV;

out vec4 outColor;

// Parametry oswietlenia
#define LIGHT_STRENGTH    2
#define LIGHT_REACH       9

#define MAX_LIGHTS        20

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

void main()
{
	// zmienna kumulujaca swiatlo punktowe
	vec3 pointLights = vec3(0.0);
    vec3 ambients = vec3(0.0);
	vec3 specularPart = vec3(0.0);

	for (int i=0; i<Number_Of_Lights; i++)
	{
		//zmniejszenie mocy wraz z odlegloscia
	    float odl = distance(vec3(ourPosition),Light_Position[i]);
		float odlMod = LIGHT_STRENGTH - (odl/LIGHT_REACH);

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
		//vec3 viewDir = normalize(Camera_Position - vec3(ourPosition));
		//vec3  reflectDir = reflect(-lightDirection, ourNormal);
		//float specularCoeff = pow(max(dot(viewDir, reflectDir), 0.0), myMaterial.Shininess);
		//vec3  resultSpecular = specularCoeff * vec3(myLight.Specular) * myMaterial.Specular;

		//specularPart = resultSpecular * odlMod;

	}
	// Zastosowanie oswietlenia do fragmentu
	vec4 objectColor = texture( tex0, inoutUV );

	vec4 result =  vec4(ambients/Number_Of_Lights, 1.0) * objectColor + vec4(pointLights, 1.0) * objectColor + vec4(specularPart/Number_Of_Lights, 1.0) * objectColor;

	outColor = result;
}
