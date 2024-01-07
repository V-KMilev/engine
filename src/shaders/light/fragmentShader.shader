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

struct Light {
	vec3 position;
	vec3 color;
};


in VS_OUT {
	float depth;
} fs_in;

uniform Camera uCamera;
uniform Light uLight;

void main() {
	gl_FragDepth = fs_in.depth;

	outColor = vec4(uLight.color, 1.0);
}
