#version 410 core

in vec2		tCoords;
in vec3		normal;
in vec3		vecToLight;
in float	typeF;
out vec4	FragColor;

uniform sampler2D	basicTexture;

void	main()
{
	float	brightness;
	vec3	lightColor = vec3(1.0, 1.0, 1.0);
	vec3	diffuse;
	vec4	textureColor;
	vec3	colorAddedTexture = vec3(1.0, 1.0, 1.0);

	brightness = max(dot(normalize(vecToLight), normalize(normal)), 0.2);
	diffuse = brightness * lightColor;

	textureColor = texture(basicTexture, tCoords);
	if (textureColor.w < 0.9)
		discard;
	if (int(typeF) == 3)
		colorAddedTexture = vec3(0.0, 1.0, 0.0);
	FragColor = vec4(diffuse, 1.0) * vec4(colorAddedTexture, 1.0) * textureColor;
}
