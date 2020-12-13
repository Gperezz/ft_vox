#version 410 core
layout (location = 0) in vec2 aPos;

uniform mat4	view;
uniform mat4	projection;
uniform int		number;

out	vec2		tCoords;

void main()
{
	vec4	pos4;
	vec2	offset;

	offset.x = number % 4;
	offset.y = number / 4;
	tCoords = (aPos + 1) / 2;
	tCoords.x = (tCoords.x / 4.0) + (1.0 / 4.0) * offset.x;
	tCoords.y = (tCoords.y / 3.0 + 2.0 / 3.0) - (1.0 / 3.0) * offset.y;
	pos4 = vec4(aPos, 0.0, 1.0);
	gl_Position = view * pos4;
}