#version 410 core

in vec2		tCoords;
in vec3		normal;
in vec3		vecToLight;
in float	textureType;
in float	type;
out vec4	FragColor;

uniform sampler2D	basicTexture;

vec3	convertRGB(vec3 rgb)
{
	return (rgb /= 255.0);
}

#define		AIR 0
#define		STONE 1
#define		DIRT 2
#define		GRASS 3
#define		LOG 4
#define		LEAVES 5
#define		WATER 6
#define		SNOW 7
#define		SAND 8

void	main()
{
	float	brightness;
	vec3	lightColor = vec3(1.0, 1.0, 1.0);
	vec3	diffuse;
	vec4	textureColor;
	vec4	colorAddedTexture = vec4(1.0, 1.0, 1.0, 1.0);

	brightness = clamp(dot(normal, vecToLight) + 0.1, 0.2, 1.);
	diffuse = brightness * lightColor;
	textureColor = texture(basicTexture, tCoords);
	if (textureColor.w < 0.9)
		discard;
	if ((int(textureType) == 1 && type == GRASS) || type == LEAVES)
		colorAddedTexture = vec4(convertRGB(vec3(176, 252, 113)), 1.0);
	else if (type == SNOW)
		textureColor = (textureColor + vec4(1., 1., 1., 1.)) / 2;
	else if (type == WATER)
		colorAddedTexture = vec4(convertRGB(vec3(0, 100, 255)), 0.7);
	FragColor = vec4(diffuse, 1.0) * colorAddedTexture * textureColor;
}
