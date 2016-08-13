uniform sampler2D texture; // Our render texture
uniform sampler2D distortionMapTexture; // Our heat distortion map texture

uniform float time; // Time used to scroll the distortion map
uniform float distortionFactor; // Factor used to control severity of the effect
uniform float riseFactor; // Factor used to control how fast air rises
uniform float limit;
uniform float offset_limit;
uniform float height;
uniform float max_factor;
uniform float activate_persistence;
uniform float offset;
uniform mat4 kernel;
uniform vec2 camera_offset;
uniform vec2 resolution;

void main()
{
	if (gl_FragCoord.y < limit)
	{
		vec2 distortionMapCoordinate = gl_TexCoord[0].st;

		// We use the time value to scroll our distortion texture upwards
		// Since we enabled texture repeating, OpenGL takes care of
		// coordinates that lie outside of [0, 1] by discarding
		// the integer part and keeping the fractional part
		// Basically performing a "floating point modulo 1"
		// 1.1 = 0.1, 2.4 = 0.4, 10.3 = 0.3 etc.
		distortionMapCoordinate.y -= camera_offset.y / resolution.y + time * riseFactor;
		distortionMapCoordinate.x += camera_offset.x / resolution.x;

		vec4 distortionMapValue = texture2D(distortionMapTexture, distortionMapCoordinate);

		// The values are normalized by OpenGL to lie in the range [0, 1]
		// We want negative offsets too, so we subtract 0.5 and multiply by 2
		// We end up with values in the range [-1, 1]
		vec2 distortionPositionOffset = distortionMapValue.xy;
		distortionPositionOffset -= vec2(0.5, 0.5);
		distortionPositionOffset *= 2.0;

		// The factor scales the offset and thus controls the severity
		distortionPositionOffset *= distortionFactor;

		// The latter 2 channels of the texture are unused... be creative
		vec2 distortionUnused = distortionMapValue.zw;

		// Since we all know that hot air rises and cools,
		// the effect loses its severity the higher up we get
		// We use the t (a.k.a. y) texture coordinate of the original texture
		// to tell us how "high up" we are and damp accordingly
		// Remember, OpenGL 0 is at the bottom
		float factor = (offset_limit - gl_TexCoord[0].y) / height;
		if (factor > max_factor)
			factor = max_factor;
		distortionPositionOffset *= factor;

		vec2 distortedTextureCoordinate = gl_TexCoord[0].st + distortionPositionOffset;

		vec3 col = texture2D(texture, distortedTextureCoordinate + vec2(-offset, offset)).rgb * kernel[0][0];
		col += texture2D(texture, distortedTextureCoordinate + vec2(0.0, offset)).rgb * kernel[1][0];
		col += texture2D(texture, distortedTextureCoordinate + vec2(offset, offset)).rgb * kernel[3][0];
		col += texture2D(texture, distortedTextureCoordinate + vec2(-offset, 0.0)).rgb * kernel[0][1];
		col += texture2D(texture, distortedTextureCoordinate + vec2(0.0, 0.0)).rgb * kernel[1][1];
		col += texture2D(texture, distortedTextureCoordinate + vec2(offset, 0.0)).rgb * kernel[3][1];
		col += texture2D(texture, distortedTextureCoordinate + vec2(-offset, -offset)).rgb * kernel[0][3];
		col += texture2D(texture, distortedTextureCoordinate + vec2(0.0, -offset)).rgb * kernel[1][3];
		col += texture2D(texture, distortedTextureCoordinate + vec2(offset, -offset)).rgb * kernel[3][3];

		gl_FragColor = mix(texture2D(texture, distortedTextureCoordinate), vec4(col, 1.0), activate_persistence * factor / max_factor);
		//gl_FragColor = mix(texture2D(distortionMapTexture, distortionMapCoordinate), vec4(col, 1.0), activate_persistence * factor / max_factor);
	}
	else
		gl_FragColor = texture2D(texture, gl_TexCoord[0].xy);
}
