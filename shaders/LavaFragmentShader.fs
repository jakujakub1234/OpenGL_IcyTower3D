#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;

uniform float transparent;
uniform float time;

float noise(in vec2 coords)
{	
	vec2 rand_coords = coords.yx * sin(coords.x / 5.0f) / 1.4f * pow(cos(coords.y / 5.0f) / 2.4f, 2.0f);
	return mix(rand_coords.x, rand_coords.y, rand_coords.x);
}

vec2 move_lava(in vec2 p)
{
	return vec2(noise(p.xy), noise(p.yx));
}

void main()
{
    FragColor = vec4(texture(texture1, TexCoord.xy + (abs(sin(time/6.5) + 1.5) / 500.0)  * move_lava(5.23 * TexCoord.xy + ((sin(time / 5.0f)) + 3.0f) * 50.0)).xyz, transparent);
}