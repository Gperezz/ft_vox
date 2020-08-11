#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float meta;

uniform mat4 view;
uniform mat4 projection;

out vec2 tCoords;

void main()
{
	vec4 pos4;

	tCoords = aPos.xy;
	pos4 = vec4(aPos.xyz, 1.0);
	gl_Position = projection * view * pos4;
}