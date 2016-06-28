uniform sampler2D texture; // Our render texture
uniform vec2 resolution;
uniform float time;
uniform float appear;
uniform float disappear;

vec2 rot(vec2 old, float ang)
{
	ang = radians(ang);
	float c = cos(ang);
	float s = sin(ang);
	return vec2(c * old.x + s * old.y, -s * old.x + c * old.y);
}

void main(void)
{
	vec2 uv = gl_TexCoord[0].xy * 2.0 - 1.0;
	uv.x *= resolution.x / resolution.y;

	float dir = step(0.0, length(uv) - 0.75) * 2.0 - 1.0;

	uv = rot(uv, time * 100.0);

	float b = smoothstep(-0.1, 0.1, cos(atan(uv.x, uv.y) * 10.0 + (length(uv) * -dir) * 75.0));

	vec2 xy = gl_TexCoord[0].xy;
	xy.y = abs(xy.y + 0.5 * sin(time) * sin(time * 0.05 * xy.y));
	xy.x = abs(xy.x + 0.5 * cos(time) * sin(time * 0.05 * xy.x));
	vec4 texColor = mix(texture2D(texture, xy), vec4(1.0), disappear);

	texColor.r *= abs(sin(time * 0.5));
	texColor.g *= abs(cos(time * 0.4));
	texColor.b *= abs(sin(time * 0.3) * cos(time));

	gl_FragColor = mix(texture2D(texture, gl_TexCoord[0].xy), texColor * b, appear);
}
