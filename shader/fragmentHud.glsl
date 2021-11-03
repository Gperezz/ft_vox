#version 410 core

in vec2		tCoords;
out vec4	FragColor;

uniform int			number;
uniform int			cursorColor;
uniform sampler2D	basicTexture;

void	main()
{
	vec4 colorT;

	colorT = texture(basicTexture, tCoords);
	if (colorT.w > 0.5)
	{
		if (number == 11)
		{
			if (cursorColor == 0)
				colorT = vec4(1.0, 1.0, 1.0, 1.0);
			else if (cursorColor == 1)
				colorT = vec4(0.0, 1.0, 0.0, 1.0);
			else if (cursorColor == 2)
				colorT = vec4(1.0, 0.0, 0.0, 1.0);
		}
		else
			colorT = vec4(1.0, 1.0, 1.0, 1.0);
	}
	FragColor = colorT;
}