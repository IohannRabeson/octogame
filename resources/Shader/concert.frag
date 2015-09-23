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
	float total = 0.0;
	float frequency = 1.0;
	float amplitude = a.x;
	for (int i = 0; i < a.z; ++i)
	{
		total += noise(p * frequency) * amplitude;
		frequency *= a.y;
		amplitude *= a.x;
	}
	return total;
}

void main()
{
	vec2 p = gl_TexCoord[0].xy * 0.2 - 1.0;
	p.x *= resolution.x / resolution.y;
	p *= 20.0;

	vec4 distorsionValue = vec4(fbm(p, vec3(0.5, 2.0, 2.0)));
	vec2 distorsionOffset = distorsionValue.xy;
	distorsionOffset -= vec2(0.5, 0.5);
	distorsionOffset *= 2.0 * 0.3 * time;

	vec2 distorsionTexCoord = gl_TexCoord[0].xy + distorsionOffset;
	gl_FragColor = texture2D(texture, distorsionTexCoord);

}

