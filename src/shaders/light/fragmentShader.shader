#version 330 core
layout(location = 0) out vec4 outColor;

struct Camera {
	mat4 projection;
	mat4 view;

	vec3 position;

	float FOV;
	float width;
	float height;

	float near;
	float far;
};

struct Light {
	vec3 position;
	vec3 color;
};

uniform Camera uCamera;
uniform Light uLight;

in VS_OUT {
	float depth;
} fs_in;

void main() {
	const float gamma = 2.2;

	gl_FragDepth = fs_in.depth;

	// Gamma correction
	vec3 color = pow(uLight.color, vec3(1.0 / gamma));

	outColor = vec4(color, 1.0);
}
