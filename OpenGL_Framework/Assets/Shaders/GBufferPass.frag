#version 420

uniform sampler2D Albedo;
uniform sampler2D Roughness;
uniform sampler2D Metallic;

in vec2 texcoord;
in vec3 norm;
in vec3 pos;

layout (location = 0) out vec4 outColor;
layout (location = 1) out vec3 outNormals;
layout (location = 2) out vec3 outPosition;

layout (location = 3) out vec4 outRoughness;
layout (location = 4) out vec4 outMetallic;

void main()
{
	//standard color output
	outColor.rgb = texture(Albedo, texcoord).rgb;
	outColor.a = 1.0;

	//roughness output
	outRoughness.rgb = texture(Roughness, texcoord).rgb;
	outRoughness.a = 1.0;

	//Metallic output
	outMetallic.rgb = texture(Metallic, texcoord).rgb;
	outMetallic.a = 1.0;

	//Pack Normals
		//in-> [-1,1]
		//out->[0,1]
	outNormals = norm * 0.5 + 0.5;

	//viewSpace Positions
	outPosition = pos;

}