uniform sampler2D tex;
uniform vec2 center;
uniform float radius;
uniform float fade_out_size;
uniform float fade_in_size;
uniform float deformation_factor;

void main (void)
{
	float len = distance(gl_FragCoord.xy, center);
	float outCircle = smoothstep(len, len + fade_out_size, radius);
	float inCircle = smoothstep(len - fade_in_size, len, radius);
	vec2 direction = normalize(center - gl_FragCoord.xy);
	vec2 newTexCoord = gl_TexCoord[0].st + (direction * inCircle * (1.0 - outCircle) * deformation_factor);
	gl_FragColor = texture2D(tex, newTexCoord);
}
