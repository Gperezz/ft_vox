#version 410 core
out vec4			FragColor;
in vec2				tCoords;
in vec4				aCol;

uniform sampler2D	basicTexture;
uniform vec2		envx;
uniform vec2		envy;
uniform vec2		envz;

void	main()
{
	FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
