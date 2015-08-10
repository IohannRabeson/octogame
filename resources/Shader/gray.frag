uniform sampler2D texture;

void main()
{
	// lookup the pixel in the texture
	vec4	pixel = texture2D(texture, gl_TexCoord[0].xy);
	float	gray = pixel.x * 0.39 + pixel.y * 0.50 + pixel.z * 0.11;

	gl_FragColor = vec4(gray, gray, gray, pixel.a);
}
