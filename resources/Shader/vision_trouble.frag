uniform sampler2D texture;
uniform vec3 offset;

void main()
{
	vec4 sample = texture2D(texture, gl_TexCoord[0].xy + offset.yz) / 3;
	sample += texture2D(texture, gl_TexCoord[0].xy + offset.xy) / 3;
	offset.y = -offset.y;
	sample += texture2D(texture, gl_TexCoord[0].xy + offset.xy * 2.0) / 3;

	gl_FragColor = sample;
}
