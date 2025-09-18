#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragmentPosition;

uniform sampler2D textureData0;

uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

void main()
{
    // general
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightPosition - FragmentPosition);

    // ambient lighting
    float ambientStrength = 0.3;
    vec3 ambientLight = lightColor * ambientStrength;

    // diffuse lighting
    vec3 diffuse  = lightColor * max(dot(normal,lightDirection), 0.0);

    // specular lighting
    float specularStrength = 0.5;
    vec3 viewDirection = normalize(cameraPosition - FragmentPosition);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    vec3 specular = specularStrength * lightColor * pow(max(dot(viewDirection, reflectionDirection), 0.0), 32);

    vec4 texture0 = texture(textureData0, TexCoord);
    FragColor = texture0 * vec4(ambientLight + diffuse + specular, 1.0);
};