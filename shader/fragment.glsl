#version 410 core

in vec2		tCoords;
in vec3		normal;
in vec3		vecToLight;
out vec4	FragColor;

uniform sampler2D	basicTexture;

void	main()
{
	float	brightness;
	vec3	lightColor = vec3(1.0, 1.0, 1.0);
	vec3	diffuse;
	vec4	textureColor;

	brightness = max(dot(normalize(vecToLight), normalize(normal)), 0.2);
	diffuse = brightness * lightColor;

	textureColor = texture(basicTexture, tCoords);
	if (textureColor.w < 0.9)
		discard;
	FragColor = vec4(diffuse, 1.0) * textureColor;
}
