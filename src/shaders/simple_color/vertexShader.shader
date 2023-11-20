#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

struct Camera {
	mat4 projection;
	mat4 view;

	float FOV;
	float width;
	float height;

	float near;
	float far;
};


uniform Camera uCamera;
uniform mat4 uModel;

void main() {
	gl_Position = uCamera.projection * uCamera.view * uModel * vec4(position, 1.0);
}
