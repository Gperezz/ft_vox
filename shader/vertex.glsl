#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in uint meta;

uniform mat4 view;
uniform mat4 world;
uniform mat4 projection;

out vec2 tCoords;

void main()
{
	vec4 pos4;

	tCoords = aPos.xz;
	pos4 = vec4(aPos.xyz, 1.0);
	if (meta == 1)
		tCoords = aPos.zy;
	else if (meta == 2)
		tCoords = aPos.xz;
	else if (meta == 3)
		tCoords = aPos.xy;
	gl_Position = projection * view * world * pos4;
}