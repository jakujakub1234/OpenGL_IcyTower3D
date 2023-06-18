#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;

uniform float transparent;

void main()
{
	FragColor = vec4(texture(texture1, TexCoord).xyz, transparent);
}