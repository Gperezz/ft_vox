#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float meta;

uniform mat4 view;
uniform mat4 projection;

out vec2 tCoords;

void main()
{
	vec4	pos4;
	int		dir;

	dir = int(meta);
	tCoords = aPos.xy;
	pos4 = vec4(aPos.xyz, 1.0);
	if (dir == 1 || dir == 6)
		tCoords = aPos.zy;
	else if (dir == 2 ||dir == 5)
		tCoords = aPos.xz;
	else
		tCoords = aPos.xy;
	gl_Position = projection * view * pos4;

}