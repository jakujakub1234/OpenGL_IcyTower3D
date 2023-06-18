#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, vec3(TexCoords.x, 1.0 - TexCoords.y, TexCoords.z));
}