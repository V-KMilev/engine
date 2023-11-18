#version 330 core
layout(location = 0) out vec4 outColor;

struct Material {
	sampler2D Ambient;
	sampler2D Diffuse;
	sampler2D Specular;
	sampler2D Specular_highlight;
	sampler2D Bump;
	sampler2D Displacement;
	sampler2D Alpha;
	sampler2D Reflection;
	sampler2D Roughness;
	sampler2D Metallic;
	sampler2D Sheen;
	sampler2D Emissive;
	sampler2D Normal;
};

in VS_OUT {
	vec4 local_position;
	vec4 camera_position;
	vec4 world_position;

	vec2 texCoords;
	vec3 normal;

	float depth;

	mat4 projection;
	mat4 view;
} fs_in;

uniform int uSelected;

uniform float uNear;
uniform float uFar;

uniform Material uMaterial;

void main() {
	vec3 localPosition = fs_in.local_position.xyz;

	gl_FragDepth = fs_in.depth;

	outColor = texture(uMaterial.Diffuse, fs_in.texCoords);// + vec4(localPosition, 1.0);

	if(bool(uSelected)) {
		outColor *= vec4(1.0, 0.7, 0.7, 0.5);
	}
}
