uniform sampler2D texture; // Our render texture
uniform vec2 bot_left;
uniform vec2 top_right;
uniform vec2 size;
uniform vec2 sub_bot_left;
uniform vec2 sub_top_right;
uniform vec2 sub_size;
uniform float offset;
uniform float reverse;
uniform mat4 kernel;

float insideBox(vec2 v, vec2 bot_left, vec2 top_right)
{
	vec2 s = step(bot_left, v) - step(top_right, v);
	return s.x * s.y;
}

vec3 kernel_color(vec2 coord)
{
	vec3 col = texture2D(texture, coord + vec2(-offset, offset)).rgb * kernel[0][0];
	col += texture2D(texture, coord + vec2(0.0, offset)).rgb * kernel[1][0];
	col += texture2D(texture, coord + vec2(offset, offset)).rgb * kernel[3][0];
	col += texture2D(texture, coord + vec2(-offset, 0.0)).rgb * kernel[0][1];
	col += texture2D(texture, coord + vec2(0.0, 0.0)).rgb * kernel[1][1];
	col += texture2D(texture, coord + vec2(offset, 0.0)).rgb * kernel[3][1];
	col += texture2D(texture, coord + vec2(-offset, -offset)).rgb * kernel[0][3];
	col += texture2D(texture, coord + vec2(0.0, -offset)).rgb * kernel[1][3];
	col += texture2D(texture, coord + vec2(offset, -offset)).rgb * kernel[3][3];
	return col;
}

void main(void)
{
	vec2 texCoord = gl_TexCoord[0].xy;
	float inBox = insideBox(texCoord, bot_left, top_right);
	float inSubBox = insideBox(texCoord, sub_bot_left, sub_top_right);

	vec2 texCoordBox = mix(texCoord, (texCoord - bot_left) / size, inBox);
	vec2 texCoordSubBox = mix(texCoordBox, (texCoord - sub_bot_left) / sub_size, inSubBox);

	vec4 col = vec4(mix(texture2D(texture, texCoordBox).rgb, kernel_color(texCoordBox), mix(inBox, 1.0 - inBox, reverse)), 1.0);
	vec4 colSub = vec4(mix(texture2D(texture, texCoordSubBox).rgb, kernel_color(texCoordSubBox), mix(1.0 - inSubBox, inSubBox, reverse)), 1.0);

	gl_FragColor = mix(col, colSub, mix(inBox, inSubBox, inSubBox));
}
