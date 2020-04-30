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

uniform sampler2D tex0;

void main()
{
	// zmienna kumulujaca swiatlo punktowe
	vec3 pointLights = vec3(0.0);

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

	}
	// Zastosowanie oswietlenia do fragmentu
	vec4 objectColor = texture( tex0, inoutUV );

	vec4 result =  vec4(Light_Ambient[0], 1.0) * objectColor + vec4(pointLights, 1.0) * objectColor;

	outColor = result;
}
