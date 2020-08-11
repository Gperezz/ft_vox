#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float meta;

uniform mat4 view;
uniform mat4 world;
uniform mat4 projection;

out vec2 tCoords;

void main()
{
	vec4	pos4;

	tCoords = aPos.zy;
	pos4 = vec4(aPos.xyz, 1.0);
	if (int(meta) == 1 || int(meta) == 6)
		tCoords = aPos.zy;
	else if (int(meta) == 2 ||int(meta) == 5)
		tCoords = aPos.xz;
	else
		tCoords = aPos.xy;
	gl_Position = projection * view * world * pos4;
}