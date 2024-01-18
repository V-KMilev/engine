#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

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
uniform mat4 uModel;

out VS_OUT {
	float depth;
} vs_out;

void main() {
	vec4 local_position = vec4(position, 1.0);
	vec4 mvp_position = uCamera.projection * uCamera.view * uModel * local_position;

	vs_out.depth = mvp_position.z / mvp_position.w;

	gl_Position = mvp_position;
}
