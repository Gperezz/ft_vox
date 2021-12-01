#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aCoords;
layout (location = 2) in int meta;

uniform mat4	view;
uniform mat4	projection;
uniform int		nbTxt;

out vec2 tCoords;

void main()
{
	vec4	pos4;
	int		dir;
	int		typeF;

	dir = meta & 7;
	typeF = (meta >> 8);
	pos4 = vec4(aPos.xyz, 1.0);
	tCoords = aCoords;
	tCoords.y = (tCoords.y / nbTxt) + (1.0 / nbTxt) * typeF;
	gl_Position = projection * view * pos4;
}