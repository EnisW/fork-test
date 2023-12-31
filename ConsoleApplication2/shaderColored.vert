#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inUV;
layout(location = 3) in float modelIndex;

out vec4 FragPosLightSpace;
out vec3 outColor;

uniform mat4 M[100];
uniform mat4 VP;
uniform mat4 lightSpaceMatrix;

void main(){
	gl_Position =	VP * M[int(modelIndex)] *  vec4(inPos ,1);
	outColor = inColor;
	FragPosLightSpace = lightSpaceMatrix * M[int(modelIndex)] * vec4(inPos, 1.0);
}