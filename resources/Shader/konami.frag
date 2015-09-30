uniform sampler2D texture;
uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

float tau = atan(1.0) * 8.0;
float hue(float x)
{
	return clamp(2.0 * cos(vec3(tau * x) + (tau * vec3(0,2,1) / 3.0)),-1.0, 1.0) * 0.5 + 0.5;
}

vec3 Hue2( float hue )
{
	vec3 rgb = fract(hue +
			vec3(
				0.0/3.0,
				2.0/3.0,
				1.0/3.0
				));

	rgb = abs(rgb*2.0-1.0);

	return clamp(rgb*3.0-1.0,0.0,1.0);
}

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
	return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec3 HSVtoRGB( vec3 hsv )
{
	return ((Hue2(hsv.x)-1.0)*hsv.y+1.0) * hsv.z;
}

void main(void)
{
	vec4 color = gl_Color * texture2D(texture, gl_TexCoord[0].xy);
	vec3 hsv = rgb2hsv(color.rgb);
	hsv.r *= time * 10.0; 
	gl_FragColor = vec4(hsv2rgb(hsv), 1.0);
	//vec2 position = (gl_FragCoord.xy / resolution.xy);

	//float len = length((gl_FragCoord.xy - resolution.xy * 0.5) * 0.003);
	//vec4 color = gl_Color * texture2D(texture, gl_TexCoord[0].xy);
	//color.r = hue(color.r + len * time);
	//color.g = hue(color.g + len * time);
	//color.b = hue(color.b + len * time);
	//color.a = 1.0;
	//gl_FragColor = color;
	//gl_FragColor = vec4(hue(mod(color - time * 0.5 , 1.)), 1.0);
}
