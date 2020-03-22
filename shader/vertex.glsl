#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 tCoords;

void main()
{
	vec4 pos4;
	pos4 = vec4(aPos, 1.0);
	tCoords = aPos.xy;
	gl_Position = projection * view * model * pos4;
}