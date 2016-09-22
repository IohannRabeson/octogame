uniform sampler2D texture; // Our render texture
uniform float transition;

void main(void)
{
	gl_FragColor = mix(texture2D(texture, gl_TexCoord[0].xy).rgba, vec4(0.6, 0.0, 0.0, 0.5), transition);
}
