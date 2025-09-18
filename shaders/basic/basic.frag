#version 330 core

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragmentPosition;

uniform sampler2D textureData0;

uniform vec3 cameraPosition;

uniform Material material;
uniform Light light;

void main()
{
    // general
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(light.position - FragmentPosition);

    // ambient lighting
    vec3 ambient = light.ambient * material.ambient;

    // diffuse lighting
    vec3 diffuse  = light.diffuse * (max(dot(normal,lightDirection), 0.0) * material.diffuse);

    // specular lighting
    vec3 viewDirection = normalize(cameraPosition - FragmentPosition);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    vec3 specular = light.specular * (pow(max(dot(viewDirection, reflectionDirection), 0.0), material.shininess) * material.specular);

    vec4 texture0 = texture(textureData0, TexCoord);
    FragColor = texture0 * vec4(ambient + diffuse + specular, 1.0);
};