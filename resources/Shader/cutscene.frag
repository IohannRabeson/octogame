uniform sampler2D texture;
uniform float height;
uniform float time;

void main()
{
	float time_step = height * time;
	float is_black = step(gl_TexCoord[0].y, time_step) + step(1.0 - time_step, gl_TexCoord[0].y);
	gl_FragColor = mix(texture2D(texture, gl_TexCoord[0].xy), vec4(vec3(0.0), 1.0), is_black);
}
