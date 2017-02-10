uniform sampler2D texture;
uniform float time;
uniform float radius;
uniform float color_size;
uniform float fade_out_size;
uniform float alpha;
uniform vec2 position;

vec3 rgb2hsv(vec3 c)
{
	vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
	vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
	vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

	float d = q.x - min(q.w, q.y);
	float e = 1.0e-10;
	return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
	return vec3(c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y));
}

void main(void)
{
	vec3 color = vec4(gl_Color * texture2D(texture, gl_TexCoord[0].xy)).rgb;
	float len = length(gl_FragCoord.xy - position);
	float inCircle = smoothstep(len - fade_out_size, len + fade_out_size, radius);

	vec3 hsv = rgb2hsv(color);
	hsv.r += len * color_size - time;
	//hsv.g = clamp(hsv.g, 0.5, 1.0);
	gl_FragColor = vec4(mix(color, hsv2rgb(hsv), inCircle * alpha), 1.0);
}
