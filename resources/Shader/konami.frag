uniform sampler2D texture;
uniform float time;
uniform float time_factor;
uniform float radius;
uniform float radius_factor;
uniform float radius_gradient;
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

float lerp(float a, float b, float alpha)
{
	return (a * (1.0 - alpha) + b * alpha);
}

void main(void)
{
	vec4 color = gl_Color * texture2D(texture, gl_TexCoord[0].xy);
	float len = length(gl_FragCoord.xy - position);

	if (len < radius)
	{
		vec3 hsv = rgb2hsv(color.rgb);
		hsv.r += len * radius_factor - time_factor * time;
		//hsv.g = clamp(hsv.g, 0.5, 1.0);
		//hsv.b = 0.8;
		vec3 rgb = vec4(hsv2rgb(hsv), 1.0);
		float alpha = clamp(abs(len - radius) / radius_gradient, 0.0, 1.0);
		gl_FragColor.r = lerp(color.r, rgb.r, alpha);
		gl_FragColor.g = lerp(color.g, rgb.g, alpha);
		gl_FragColor.b = lerp(color.b, rgb.b, alpha);
		gl_FragColor.a = 1.0;
	}
	else
		gl_FragColor = color;
}

