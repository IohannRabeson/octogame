uniform sampler2D texture; // Our render texture
uniform float intensity;
uniform float offset;
uniform mat4 kernel;

void main()
{
	vec3 col = texture2D(texture, gl_TexCoord[0].xy + vec2(-offset, offset)).rgb * kernel[0][0];
	col += texture2D(texture, gl_TexCoord[0].xy + vec2(0.0, offset)).rgb * kernel[1][0];
	col += texture2D(texture, gl_TexCoord[0].xy + vec2(offset, offset)).rgb * kernel[3][0];
	col += texture2D(texture, gl_TexCoord[0].xy + vec2(-offset, 0.0)).rgb * kernel[0][1];
	col += texture2D(texture, gl_TexCoord[0].xy + vec2(0.0, 0.0)).rgb * kernel[1][1];
	col += texture2D(texture, gl_TexCoord[0].xy + vec2(offset, 0.0)).rgb * kernel[3][1];
	col += texture2D(texture, gl_TexCoord[0].xy + vec2(-offset, -offset)).rgb * kernel[0][3];
	col += texture2D(texture, gl_TexCoord[0].xy + vec2(0.0, -offset)).rgb * kernel[1][3];
	col += texture2D(texture, gl_TexCoord[0].xy + vec2(offset, -offset)).rgb * kernel[3][3];

	gl_FragColor = mix(texture2D(texture, gl_TexCoord[0].xt), vec4(col, 1.0), intensity);
}
