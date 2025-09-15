#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;

uniform sampler2D textureData0;

uniform vec3 ambientLightColor;

void main()
{
    float ambientStrength = 0.3;
    vec3 ambientLight = ambientLightColor * ambientStrength;

    vec4 texture0 = texture(textureData0, TexCoord);
    FragColor = texture0 * vec4(ambientLight, 1.0);
};