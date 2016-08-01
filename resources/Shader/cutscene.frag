uniform sampler2D texture;

void main()
{
	if (gl_TexCoord[0].y < 0.1 || gl_TexCoord[0].y > 0.9)
		gl_FragColor = vec4(vec3(0.0), 1.0);
	else
		gl_FragColor = texture2D(texture, gl_TexCoord[0].xy);
}
