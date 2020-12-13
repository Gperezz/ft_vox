#version 410 core

in vec2		tCoords;
out vec4	FragColor;

uniform sampler2D	basicTexture;

void	main()
{
	vec4 colorT;

	colorT = texture(basicTexture, tCoords);
	if (colorT.w > 0.5)
		colorT = vec4(1.0, 1.0, 1.0, 1.0);
	FragColor = colorT;
}