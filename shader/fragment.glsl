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

void	main()
{
	float	brightness;
	vec3	lightColor = vec3(1.0, 1.0, 1.0);
	vec3	diffuse;
	vec4	textureColor;
	vec3	colorAddedTexture = vec3(1.0, 1.0, 1.0);

	brightness = clamp(dot(normal, vecToLight) + 0.2, 0.2, 1.);
	diffuse = brightness * lightColor;
	textureColor = texture(basicTexture, tCoords);
	if (textureColor.w < 0.9)
		discard;
	if ((int(textureType) == 1 && type == 2) || type == LEAVES)
		colorAddedTexture = convertRGB(vec3(176, 252, 113));
	else if ((int(textureType) == 1 && type == SNOW))
		colorAddedTexture = vec3(1., 0., 1.);
	FragColor = vec4(diffuse, 1.0) * vec4(colorAddedTexture, 1.0) * textureColor;
}
