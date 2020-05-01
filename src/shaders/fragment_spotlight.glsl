#version 150 core

#define LIGHT_ATTENUATION 0.01
#define LIGHT_SPECULAR    vec3(2.0, 2.0, 2.0)
#define LIGHT_CUTOFF      0.7
#define LIGHT_DIRECTION   vec3(0.2f, 1.0f, 0.3f)

#define MAX_LIGHTS        20

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

void main()
{
	// zmienna kumulujaca swiatlo punktowe
	vec3 pointLights = vec3(0.0);
    vec3 ambients = vec3(0.0);
	vec3 specularPart = vec3(0.0);

	for (int i=0; i<Number_Of_Lights; i++)
	{
        // Obliczenie wektora (swiatlo - wierzcholek)
        // czyli kierunku padania swiatla na wierzcholek
        vec3 lightDirection = normalize(Light_Position[i] - vec3(ourPosition));

        float theta = dot(lightDirection, normalize(LIGHT_DIRECTION));

        if(theta > LIGHT_CUTOFF)
        {
            //zmniejszenie mocy wraz z odlegloscia
            float odl = distance(vec3(ourPosition),Light_Position[i]);
            float odlMod = 1.0 / (1.0 + LIGHT_ATTENUATION * (odl * odl));

            // obliczenie kata pomiedzy wektorem lightDir oraz wektorem normalnym
            // wartosc kata okresla pod jakim katem padaja promienie
            float lightCoeff = max(dot(ourNormal, lightDirection), 0.0);

            // obliczenie skladowej diffuse
            pointLights += lightCoeff * Light_Diffuse[i] * odlMod;

            // Specular
            vec3 viewDir = normalize(Camera_Position - vec3(ourPosition));
            vec3  reflectDir = reflect(-lightDirection, ourNormal);
            float specularCoeff = pow(max(dot(viewDir, reflectDir), 0.0), myMaterial.Shininess);
            vec3  resultSpecular = specularCoeff * LIGHT_SPECULAR * myMaterial.Specular;

            specularPart += resultSpecular * odlMod;
        }

        ambients += Light_Ambient[i];

	}
	// Zastosowanie oswietlenia do fragmentu
	vec4 objectColor = texture( tex0, inoutUV );

	vec4 result =  (vec4(ambients/Number_Of_Lights, 1.0) + vec4(pointLights, 1.0)+ vec4(specularPart, 1.0)) * objectColor;

	outColor = result;
}
