#version 330 core

out vec4 FragColor;



in vec3 outColor;
flat in uint outModelIndex;
in vec2 outUV;

uniform sampler2D sampler;

void main()
{	
	FragColor = (texture(sampler, outUV) * vec4(1.0, 1.0, 1.0, 1.0));   
}
