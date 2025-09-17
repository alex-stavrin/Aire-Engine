#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragmentPosition;

uniform sampler2D textureData0;

uniform vec3 lightColor;
uniform vec3 lightPosition;

void main()
{
    float ambientStrength = 0.3;
    vec3 ambientLight = lightColor * ambientStrength;

    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightPosition - FragmentPosition);
    vec3 diffuse  = lightColor * max(dot(normal,lightDirection), 0.0);

    vec4 texture0 = texture(textureData0, TexCoord);
    FragColor = texture0 * vec4(ambientLight + diffuse, 1.0);
};