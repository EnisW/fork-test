#version 330 core
in vec4 FragPosLightSpace;

out float fragDepth;

uniform sampler2D shadowMap;
in vec3 outColor;

out vec4 FragColor;

void main()
{
    // Transform fragment position to light space coordinates
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;

    // Map coordinates to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // Get depth value from the shadow map
    float depth = texture(shadowMap, projCoords.xy).r;

    // Compare depth to the fragment's depth
    fragDepth = projCoords.z;
    float shadow = (fragDepth - 0.001 > depth) ? 1.0 : 0.0;

    // Output final color with shadow
    // You can use this shadow value to attenuate the fragment's color
    vec3 finalColor = outColor * shadow;
    FragColor = vec4(finalColor, 1.0);
}
