#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inUV;
layout(location = 4) in float modelIndex;

out vec3 outColor;
out vec2 outUV;
flat out  uint outModelIndex;

uniform mat4 P;


void main(){
	outModelIndex = uint(modelIndex);
	gl_Position = vec4(inPos ,1);
	outColor = inColor;
	outUV = inUV;
}