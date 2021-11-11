#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aCoords;
layout (location = 3) in float meta;

uniform mat4	view;
uniform mat4	projection;
uniform int		nbTxt;

#define		AIR 0
#define		STONE 1
#define		DIRT 2
#define		GRASS 3
#define		LOG 4
#define		LEAVES 5
#define		WATER 6
#define		SNOW 7

out vec3	normal;
out vec3	vecToLight;
out vec2	tCoords;
out float	textureType;
out float	type;

void main()
{
	vec4	pos4;
	int		dir;
	vec3	lightP = vec3(0., 1., 0.);

	dir = int(meta) & 7;
	textureType = (int(meta) >> 3) & 31;
	type = int(meta) >> 8;
	if (dir == 2 && (type == DIRT || type == SNOW))
		textureType++;
	else if (type == DIRT && dir != 2 && dir != 5)
		textureType += 2;
	pos4 = vec4(aPos.xyz, 1.0);
	tCoords = aCoords;
	tCoords.y = (tCoords.y / nbTxt) + (1.0 / nbTxt) * textureType;
	normal = normalize(aNorm);
	vecToLight = normalize(lightP);
	gl_Position = projection * view * pos4;
}