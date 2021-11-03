#version 410 core

#define DIRT 0

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aCoords;
layout (location = 3) in float meta;

uniform mat4	view;
uniform mat4	projection;
uniform int		nbTxt;

out vec3	normal;
out vec3	vecToLight;
out vec2	tCoords;
out float	typeF;

void main()
{
	vec4	pos4;
	int		dir;
	vec3	lightP = vec3(0, 500.0, 0.0);

	dir = int(meta) & 7;
	typeF = int(meta) >> 8;
	if (typeF == DIRT && dir == 2)
		typeF++;
	else if (typeF == DIRT && dir != 2 && dir != 5)
		typeF += 2;
	pos4 = vec4(aPos.xyz, 1.0);
	tCoords = aCoords;
	tCoords.y = (tCoords.y / nbTxt) + (1.0 / nbTxt) * typeF;
	normal = aNorm;
	vecToLight = lightP - aPos;
	gl_Position = projection * view * pos4;
}