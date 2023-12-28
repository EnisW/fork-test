#version 330 core
out vec3 color;
in vec3 outColor;
in vec2 texCoord;
uniform sampler2D tex;

void main(){
  color = texture(tex,texCoord).rgb;
}