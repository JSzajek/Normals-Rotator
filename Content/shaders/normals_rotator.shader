#shader vertex
#version 420

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_TexCoords;

layout(location = 0) out vec2 TexCoords;

uniform int flip_h_img = 0;
uniform int flip_v_img = 0;

void main()
{
	TexCoords = a_TexCoords;

	if (flip_h_img == 1) // Horizontal Image Flip
	{
		TexCoords.x = 1.0 - TexCoords.x;
	}
	if (flip_v_img == 1) // Vertical Image Flip
	{
		TexCoords.y = 1.0 - TexCoords.y;
	}

	gl_Position = vec4(a_Position.x, a_Position.y, 0.0f, 1.0f);
}

#shader fragment
#version 420
			
layout(location = 0) out vec4 Color;

layout(location = 0) in vec2 TexCoords;

layout (binding = 0) uniform sampler2D normalMap;

uniform float rotation_rad;
uniform int flip_h_img = 0;
uniform int flip_v_img = 0;

uniform int flip_r_channel = 0;
uniform int flip_g_channel = 0;
uniform int flip_b_channel = 0;

vec3 rotateNormalMapVector(vec3 input, float radians)
{
	float c = cos(radians);
	float s = sin(radians);

	// Change the range from 0...1 -> -1...1
	vec2 inputNormalsXY = input.xy;
	inputNormalsXY *= 2;
	inputNormalsXY -= 1;

	// Rotate the normal map in the XY plane
	mat2 rotMat = mat2(c, -s, s, c); // Inverted rotation
	vec2 rotatedNormalMapXY = rotMat * inputNormalsXY;

	// Shift the range back from -1...1 -> 0...1
	rotatedNormalMapXY += 1;
	rotatedNormalMapXY /= 2;

	vec3 rotatedNormal = vec3(rotatedNormalMapXY, input.z);
	return rotatedNormal;
}

void main()
{
	Color = vec4(1);
	vec4 textureColor = texture(normalMap, TexCoords);
	
	if (flip_r_channel == 1)
	{
		textureColor.r = 1.0 - textureColor.r;
	}
	if (flip_g_channel == 1)
	{
		textureColor.g = 1.0 - textureColor.g;
	}
	if (flip_b_channel == 1)
	{
		textureColor.b = 1.0 - textureColor.b;
	}

	if (flip_h_img == 1) // Horizontal Image Flip
	{
		textureColor.r = 1.0 - textureColor.r;
	}
	if (flip_v_img == 1) // Vertical Image Flip
	{
		textureColor.g = 1.0 - textureColor.g;
	}

	vec3 rotatedNorm = rotateNormalMapVector(textureColor.rgb, rotation_rad);

	Color = vec4(rotatedNorm, textureColor.a);

	
}
