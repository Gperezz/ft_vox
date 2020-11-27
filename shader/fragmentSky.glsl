#version 410 core

in vec2		tCoords;
out vec4	FragColor;

uniform sampler2D	basicTexture;

void	main()
{
	FragColor = texture(basicTexture, tCoords);
}