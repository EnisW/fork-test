#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inUV;
layout(location = 3) in float modelIndex;

out vec3 outColor;
out vec2 UV;

uniform mat4 M[100];
uniform mat4 VP;

void main(){
	gl_Position =	VP *M[int(modelIndex)] *  vec4(inPos ,1);
	outColor = inColor;
	UV = inUV;
}