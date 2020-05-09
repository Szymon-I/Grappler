#version 330

layout( location = 0 ) in vec4 inPosition;
layout( location = 1 ) in vec2 inUV;
layout( location = 2 ) in vec3 inNormal;
layout( location = 3 ) in mat4 matModelInst;

uniform mat4 matProj;
uniform mat4 matView;
uniform mat4 matModel;
uniform float move;

out vec4 inoutPos;
out vec2 inoutUV;

void main()
{

	gl_Position = matProj * matView * matModelInst * inPosition * matModel;
    gl_Position.y -= move;

	inoutPos = inPosition;
	inoutUV = inUV;
}
