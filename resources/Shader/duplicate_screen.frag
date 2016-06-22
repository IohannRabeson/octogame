uniform sampler2D texture; // Our render texture
uniform vec2 bot_left;
uniform vec2 top_right;
uniform vec2 size;
uniform vec2 sub_bot_left;
uniform vec2 sub_top_right;
uniform vec2 sub_size;
uniform float offset;
uniform float reverse;
uniform float line_progress;
uniform mat4 kernel;

float insideBox(vec2 v, vec2 bot_left, vec2 top_right)
{
	vec2 s = step(bot_left, v) - step(top_right, v);
	return s.x * s.y;
}

vec3 kernel_color(vec2 coord)
{
	vec3 col = texture2D(texture, coord + vec2(-offset, offset)).rgb * kernel[0][0];
	col += texture2D(texture, coord + vec2(0.0, offset)).rgb * kernel[1][0];
	col += texture2D(texture, coord + vec2(offset, offset)).rgb * kernel[3][0];
	col += texture2D(texture, coord + vec2(-offset, 0.0)).rgb * kernel[0][1];
	col += texture2D(texture, coord + vec2(0.0, 0.0)).rgb * kernel[1][1];
	col += texture2D(texture, coord + vec2(offset, 0.0)).rgb * kernel[3][1];
	col += texture2D(texture, coord + vec2(-offset, -offset)).rgb * kernel[0][3];
	col += texture2D(texture, coord + vec2(0.0, -offset)).rgb * kernel[1][3];
	col += texture2D(texture, coord + vec2(offset, -offset)).rgb * kernel[3][3];
	return col;
}

float cross(vec2 start, vec2 end, vec2 point)
{
	return (point.x - start.x) * (end.y - start.y) - (point.y - start.y) * (end.x - start.x);
}

vec2 line_intersect(vec2 p0, vec2 p1, vec2 p2, vec2 p3)
{
	vec2 s1 = p1 - p0;
	vec2 s2 = p3 - p2;

	float s = (-s1.y * (p0.x - p2.x) + s1.x * (p0.y - p2.y)) / (-s2.x * s1.y + s1.x * s2.y);
	float t = ( s2.x * (p0.y - p2.y) - s2.y * (p0.x - p2.x)) / (-s2.x * s1.y + s1.x * s2.y);

	if (s >= 0.0 && s <= 1.0 && t >= 0.0 && t <= 1.0)
	{
		return vec2(p0.x + (t * s1.x), p0.y + (t * s1.y));
	}
	return vec2(-1.0);
}

void main(void)
{
	vec2 texCoord = gl_TexCoord[0].xy;
	float inBox = insideBox(texCoord, bot_left, top_right);

	vec2 texCoordBox = mix(texCoord, (texCoord - bot_left) / size, inBox);

	vec4 col = vec4(mix(texture2D(texture, texCoordBox).rgb, kernel_color(texCoordBox), mix(inBox, 1.0 - inBox, reverse)), 1.0);

	vec2 tl = vec2(bot_left.x, top_right.y);
	vec2 br = vec2(top_right.x, bot_left.y);
	if (inBox)
		gl_FragColor = col;
	else
	{
		if (cross(tl, vec2(0.0, 1.0), texCoord) < 0.0 && cross(bot_left, vec2(0.0), texCoord) > 0.0) // left
		{
			float sideSize = mix(1.0, size.y, texCoord.x / bot_left.x);
			vec2 start = line_intersect(bot_left, vec2(0.0, 0.0), vec2(texCoord.x, 0.0), vec2(texCoord.x, 1.0));
			float t = (texCoord.y - start.y) / sideSize;
			if (length(vec2(bot_left.x, bot_left.y + t * size.y) - texCoord) / length(vec2(bot_left.x, bot_left.y + t * size.y) - vec2(0.0, t)) < line_progress)
				gl_FragColor = vec4(mix(texture2D(texture, vec2(0.0, t)).rgb, kernel_color(vec2(0.0, t)), 1.0 - reverse), 1.0);
			else
				gl_FragColor = col;
		}
		else if (cross(tl, vec2(0.0, 1.0), texCoord) > 0.0 && cross(top_right, vec2(1.0), texCoord) < 0.0) // top
		{
			float sideSize = mix(1.0, size.x, (1.0 - texCoord.y) / (1.0 - top_right.y));
			vec2 start = line_intersect(tl, vec2(0.0, 1.0), vec2(0.0, texCoord.y), vec2(1.0, texCoord.y));
			float t = (texCoord.x - start.x) / sideSize;
			if (length(vec2(tl.x + t * size.x, tl.y) - texCoord) / length(vec2(tl.x + t * size.x, tl.y) - vec2(t, 1.0)) < line_progress)
				gl_FragColor = vec4(mix(texture2D(texture, vec2(t, 1.0)).rgb, kernel_color(vec2(t, 1.0)), 1.0 - reverse), 1.0);
			else
				gl_FragColor = col;
		}
		else if (cross(top_right, vec2(1.0), texCoord) > 0.0 && cross(br, vec2(1.0, 0.0), texCoord) < 0.0) // right
		{
			float sideSize = mix(1.0, size.y, (1.0 - texCoord.x) / (1.0 - top_right.x));
			vec2 start = line_intersect(br, vec2(1.0, 0.0), vec2(texCoord.x, 0.0), vec2(texCoord.x, 1.0));
			float t = (texCoord.y - start.y) / sideSize;
			if (length(vec2(br.x, br.y + t * size.y) - texCoord) / length(vec2(br.x, br.y + t * size.y) - vec2(1.0, t)) < line_progress)
				gl_FragColor = vec4(mix(texture2D(texture, vec2(1.0, t)).rgb, kernel_color(vec2(1.0, t)), 1.0 - reverse), 1.0);
			else
				gl_FragColor = col;
		}
		else if (cross(br, vec2(1.0, 0.0), texCoord) > 0.0 && cross(bot_left, vec2(0.0), texCoord) < 0.0) // bottom
		{
			float sideSize = mix(1.0, size.x, texCoord.y / bot_left.y);
			vec2 start = line_intersect(bot_left, vec2(0.0, 0.0), vec2(0.0, texCoord.y), vec2(1.0, texCoord.y));
			float t = (texCoord.x - start.x) / sideSize;
			if (length(vec2(bot_left.x + t * size.x, bot_left.y) - texCoord) / length(vec2(bot_left.x + t * size.x, bot_left.y) - vec2(t, 0.0)) < line_progress)
				gl_FragColor = vec4(mix(texture2D(texture, vec2(t, 0.0)).rgb, kernel_color(vec2(t, 0.0)), 1.0 - reverse), 1.0);
			else
				gl_FragColor = col;
		}
		else
			gl_FragColor = col;
	}
}
