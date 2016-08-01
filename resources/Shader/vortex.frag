uniform sampler2D texture; // Our render texture

uniform float time;
uniform float time_max;
uniform float radius;
uniform vec2 center;
uniform vec2 resolution;
uniform vec4 center_color;

const float PI = 3.14159265358979323846264;

float cerp(float a, float b, float alpha)
{
	return mix(a, b, 0.5 - (cos(alpha * PI) / 2.0));
}

void main()
{
	vec2 textureCoord = gl_TexCoord[0].xy;
	float distFromCenter = length(center - gl_FragCoord.xy);
	vec2 position = center / resolution;
	float opacity = 0.0;

	if (distFromCenter <= radius)
	{
		vec2 distCenter = gl_TexCoord[0].xy - position;
		opacity = abs(distFromCenter / radius);
		float dist = mix(-time * PI, 0.0, opacity);
		float cosV = cos(dist);
		float sinV = sin(dist);
		textureCoord.x = position.x + distCenter.x * cosV - distCenter.y * sinV;
		textureCoord.y = position.y + distCenter.x * sinV + distCenter.y * cosV;
		opacity = cerp(0.0, 1.0 - opacity, time / time_max);
	}
	gl_FragColor = vec4(mix(texture2D(texture, textureCoord).rgb, center_color.rgb, opacity), texture2D(texture, textureCoord).a);
}
