uniform sampler2D texture;
uniform float time;
uniform vec2 resolution;

float hash(float n) { return fract(sin(n) * 753.5453123); }

float noise(vec2 x)
{
	vec2 p = floor(x);
	vec2 f = fract(x);
	f = f * f * (3.0 - 2.0 * f);
	float n = p.x + p.y * 157.0;
	return mix(mix(hash(n), hash(n + 1.0), f.x), mix(hash(n + 157.0), hash(n + 158.0), f.x), f.y);
}

float fbm(vec2 p, vec3 a)
{
	float total = noise(p * 1.0) * 0.5;
	total += noise(p * a.y) * 0.25;
	return total;
}

void main()
{
	vec2 p = gl_TexCoord[0].xy * 0.2 - 1.0;
	p.x *= resolution.x / resolution.y;
	p *= 20.0;

	vec4 distortionValue = vec4(fbm(p, vec3(0.5, 2.0, 2.0)));
	vec2 distortionOffset = distortionValue.xy;
	distortionOffset -= vec2(0.5, 0.5);
	distortionOffset *= 2.0 * 0.3 * time;

	vec2 distortionTexCoord = gl_TexCoord[0].xy + distortionOffset;
	gl_FragColor = texture2D(texture, distortionTexCoord);
}
