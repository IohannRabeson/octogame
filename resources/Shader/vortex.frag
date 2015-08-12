uniform sampler2D texture; // Our render texture

uniform float time;
uniform float time_max;
uniform float radius;
uniform vec2 center;
uniform vec2 resolution;

const float PI = 3.14159265358979323846264;

float lerp(float a, float b, float alpha)
{
	return (a * (1.0 - alpha) + b * alpha);
}

float cerp(float a, float b, float alpha)
{
	return lerp(a, b, 0.5 - (cos(alpha * PI) / 2.0));
}

void main()
{
	vec2 textureCoord = gl_TexCoord[0].xy;
	float distFromCenter = length(center - gl_FragCoord.xy);
	center.x = center.x / resolution.x;
	center.y = center.y / resolution.y;
	float opacity = 0;

	if (distFromCenter <= radius)
	{
		vec2 distCenter = gl_TexCoord[0].xy - center;
		opacity = abs(distFromCenter / radius);
		float dist = lerp(-time * PI, 0.0, opacity);
		float cosV = cos(dist);
		float sinV = sin(dist);
		textureCoord.x = center.x + distCenter.x * cosV - distCenter.y * sinV;
		textureCoord.y = center.y + distCenter.x * sinV + distCenter.y * cosV;
		opacity = cerp(0, 1 - opacity, time / time_max);
	}
	gl_FragColor = gl_Color * texture2D(texture, textureCoord);
	gl_FragColor.r = lerp(gl_FragColor.r, 0, opacity);
	gl_FragColor.g = lerp(gl_FragColor.g, 0, opacity);
	gl_FragColor.b = lerp(gl_FragColor.b, 0, opacity);
}
