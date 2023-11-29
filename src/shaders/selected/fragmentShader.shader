#version 330 core
layout(location = 0) out vec4 outColor;

struct Camera {
	mat4 projection;
	mat4 view;

	float FOV;
	float width;
	float height;

	float near;
	float far;
};

in VS_OUT {
	vec4 local_position;
	vec4 world_position;
	vec4 mvp_position;

	vec2 texCoords;
	vec3 normal;

	float depth;
} fs_in;

uniform int uSelected;
uniform Camera uCamera;

void main() {
	vec3 localPosition = fs_in.local_position.xyz;

	gl_FragDepth = fs_in.depth;

	outColor = vec4(1.0, 0.2, 0.0, 1.0);
}
