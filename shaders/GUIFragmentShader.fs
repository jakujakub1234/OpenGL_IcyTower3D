#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;
uniform float score_digit_by_10;

void main()
{    
    vec2 act_tex_coords;

    if (score_digit_by_10 < -0.5) {
        act_tex_coords = TexCoords;
    }
    else  {
        float y = (0.9 - score_digit_by_10) + (TexCoords.y / 10.0);
        act_tex_coords = vec2(TexCoords.x, y);
    }

    vec4 texColor = texture(texture1, act_tex_coords);

    if (texColor.a < 0.1) {
        discard;
    }

    FragColor = texColor;
}