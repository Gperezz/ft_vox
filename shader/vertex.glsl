#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in int meta;

void main()
{
	vec4 pos4;

	pos4 = vec4(aPos.xyz, 1.0);
	gl_Position = pos4;
}