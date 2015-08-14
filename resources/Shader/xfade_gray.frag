uniform sampler2D	texture;
uniform float		crossfade;

const float PI = 3.14159265358979323846264;

float lerp(float a, float b, float alpha)
{
	return (a * (1.0 - alpha) + b * alpha);
}

void main()
{
	vec4	pixel = texture2D(texture, gl_TexCoord[0].xy);
	float	gray = pixel.x * 0.39 + pixel.y * 0.50 + pixel.z * 0.11;
	float	interCrossfade = sin(crossfade * PI);
	gl_FragColor = vec4(lerp(pixel.r, gray, interCrossfade),
						lerp(pixel.g, gray, interCrossfade),
						lerp(pixel.b, gray, interCrossfade),
						pixel.a);
}
