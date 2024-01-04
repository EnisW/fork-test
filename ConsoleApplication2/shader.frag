#version 330 core


uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 outColor;
in vec3 outNormal;
in vec3 fragPos;
in vec3 fragPosRaw;
out vec4 FragColor;
flat in uint outModelIndex;
in vec2 outUV;

uniform sampler2D sampler[32];

void main()
{
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 ambient = 0.1 * lightColor;


    vec3 norm = normalize(outNormal);
    vec3 lightDir = normalize(lightPos - fragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    

    float specularStrength = 0.8;

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    

  
    vec3 result = (diffuse + ambient + specular) * texture(sampler[outModelIndex], outUV).rgb;
    
    FragColor = vec4(result, 1.0);
    
}
