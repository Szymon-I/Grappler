#version 150 core

in vec4 ourPosition;
in vec3 ourNormal;
in vec2 inoutUV;

out vec4 outColor;


// Parametry oswietlenia
uniform vec3 Light_Ambient;
uniform vec3 Light_Diffuse;
uniform vec3 Light_Position;

uniform sampler2D tex0;

void main()
{
	// Obliczenie wektora (swiatlo - wierzcholek)
	// czyli kierunku padania swiatla na wierzcholek
	float dist = distance(vec3(ourPosition),Light_Position);
	vec3 lightDirection = normalize(Light_Position - vec3(ourPosition));

	// obliczenie kata pomiedzy wektorem lightDir oraz wektorem normalnym
	// wartosc kata okresla pod jakim katem padaja promienie
	float lightCoeff = max(dot(ourNormal, lightDirection), 0.0);

	// obliczenie skladowej diffuse
	vec3 resultDiffuse = lightCoeff * Light_Diffuse;

	// Zastosowanie oswietlenia do fragmentu
	vec3 result = (Light_Ambient + resultDiffuse) * 1/dist;

	vec4 tex = texture( tex0, inoutUV );

	outColor = vec4(result, 1.0) * tex;

}
