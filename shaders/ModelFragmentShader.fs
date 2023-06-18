#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

uniform float jump;

void main()
{
    vec4 texColor = texture(texture1, vec2(TexCoords.x + jump / 10.0, TexCoords.y));

    if (texColor.a < 0.1) {
        discard;
    }

    FragColor = texColor;
}