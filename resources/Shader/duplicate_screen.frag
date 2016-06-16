uniform sampler2D texture; // Our render texture
uniform vec2 top_left;
uniform vec2 bot_right;
uniform vec2 size;
uniform float offset;
uniform mat4 kernel;

float insideBox(vec2 v, vec2 top_left, vec2 bot_right)
{
	vec2 s = step(top_left, v) - step(bot_right, v);
	return s.x * s.y;
}

float insideSmoothBox(vec2 v, vec2 top_left, vec2 bot_right, float t)
{
	vec2 s = smoothstep(top_left, top_left + t, v) - smoothstep(bot_right - t, bot_right, v);
	return s.x * s.y;
}

void main(void)
{
	float inBox = insideBox(gl_TexCoord[0].xy, top_left, bot_right);
	vec2 textureCoord = (gl_TexCoord[0].xy - top_left) / size;
	vec3 col = texture2D(texture, textureCoord + vec2(-offset, offset)).rgb * kernel[0][0];
	col += texture2D(texture, textureCoord + vec2(0.0, offset)).rgb * kernel[1][0];
	col += texture2D(texture, textureCoord + vec2(offset, offset)).rgb * kernel[3][0];
	col += texture2D(texture, textureCoord + vec2(-offset, 0.0)).rgb * kernel[0][1];
	col += texture2D(texture, textureCoord + vec2(0.0, 0.0)).rgb * kernel[1][1];
	col += texture2D(texture, textureCoord + vec2(offset, 0.0)).rgb * kernel[3][1];
	col += texture2D(texture, textureCoord + vec2(-offset, -offset)).rgb * kernel[0][3];
	col += texture2D(texture, textureCoord + vec2(0.0, -offset)).rgb * kernel[1][3];
	col += texture2D(texture, textureCoord + vec2(offset, -offset)).rgb * kernel[3][3];

	gl_FragColor = mix(texture2D(texture, gl_TexCoord[0]), vec4(col, 1.0), inBox);
}
