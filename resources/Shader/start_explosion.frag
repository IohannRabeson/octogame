uniform sampler2D texture;
uniform float time;
uniform float intensity;

vec2 hash2v (vec2 p)
{
	mat2 m = mat2(15.0, 35.7, 73.9, 145.3);
	return fract(sin(p*m)*49379.37);
}

float voronoi (vec2 p)
{
	vec2 g = floor(p);
	vec2 f = fract(p);
	float res = 8.0;
	for (float y=-1.0; y<=1.0; y+=1.0)
	{
		for (float x=-1.0; x<=1.0; x+=1.0)
		{
			vec2 b = vec2(x, y);
			float h = distance(hash2v(g+b)+b, f);
			res = min(res, h);
		}
	}
	return res;
}

float sdPlane(vec2 p)
{
	return voronoi(vec2(p.s, p.t*1.0)*1.0)*0.5;
}

void main(void)
{
	vec2 p = gl_TexCoord[0].xy * 0.2 - 1.0;
	p *= 40.0 * time;

	vec4 distortionValue = vec4(sdPlane(p));
	vec2 distortionOffset = distortionValue.xy;
	distortionOffset -= vec2(0.5, 0.5);
	distortionOffset *= 2.0 * 0.05 * intensity;

	vec2 distortionTexCoord = gl_TexCoord[0].xy + distortionOffset;
	gl_FragColor = texture2D(texture, distortionTexCoord);
}
