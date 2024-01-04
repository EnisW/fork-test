#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inUV;
layout(location = 4) in float modelIndex;

out vec3 fragPos;
out vec3 outColor;
out vec3 outNormal;
out vec2 outUV;
flat out  uint outModelIndex;

uniform mat4 M[100];
uniform mat4 VP;
uniform mat4 lightSpaceMatrix;

void main(){
	outModelIndex = uint(modelIndex);
	gl_Position =	VP * M[int(modelIndex)] *  vec4(inPos ,1);
	outColor = inColor;
	outUV = inUV;
	outNormal = mat3(transpose(inverse(M[int(modelIndex)]))) * inNormal;
	fragPos = vec3(M[int(modelIndex)] * vec4(inPos, 1));
}