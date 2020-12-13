#version 410 core

#define DIRT 0

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in float meta;

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
	vec3	lightP = vec3(0, 50.0, 0.0);

	dir = int(meta) & 7;
	typeF = int(meta) >> 8;
	if (typeF == DIRT && dir == 2)
		typeF++;
	if (typeF == DIRT && dir != 2 && dir != -2)
		typeF += 2;
	tCoords = aPos.zy;
	pos4 = vec4(aPos.xyz, 1.0);
	if (dir == 1 || dir == 6)
		tCoords = aPos.zy;
	else if (dir == 2 ||dir == 5)
		tCoords = aPos.xz;
	else
		tCoords = aPos.xy;
	normal = aNorm;
	vecToLight = lightP - aPos;
	// tCoords.x = mod(tCoords.x, 16);
	tCoords.y = mod(tCoords.y, 16 / nbTxt);
	tCoords.y = ((tCoords.y + typeF) / nbTxt);
	// tCoords.y += typeF * 16;
	gl_Position = projection * view * pos4;
}