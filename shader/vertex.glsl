#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float meta;

uniform mat4	view;
uniform mat4	projection;
uniform int		nbTxt;

out vec2 tCoords;

void main()
{
	vec4	pos4;
	int		dir;
	int		type;

	dir = int(meta) & 7;
	type = int(meta) >> 8;
	tCoords = aPos.zy;
	pos4 = vec4(aPos.xyz, 1.0);
	if (dir == 1 || dir == 6)
		tCoords = aPos.zy;
	else if (dir == 2 ||dir == 5)
		tCoords = aPos.xz;
	else
		tCoords = aPos.xy;
	// tCoords.x = mod(tCoords.x, 16);
	tCoords.y = mod(tCoords.y, 16 / nbTxt);
	tCoords.y = ((tCoords.y + type) / nbTxt);
	gl_Position = projection * view * pos4;
}