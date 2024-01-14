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

uniform Camera uCamera;
uniform mat4 uModel;

out VS_OUT {
	vec4 mvp_position;

	float depth;
} vs_out;

mat4 scale(vec3 scale) {
	return mat4(
		scale.x, 0.0, 0.0, 0.0,
		0.0, scale.y, 0.0, 0.0,
		0.0, 0.0, scale.z, 0.0,
		0.0, 0.0, 0.0, 1.0
	);
}

void main() {
	// TODO: Make the scale factor an uniform
	mat4 scaleMatrix = scale(vec3(1.02, 1.02, 1.02));
	mat4 model = uModel * scaleMatrix;

	vec4 local_position = vec4(position, 1.0);
	vec4 mvp_position = uCamera.projection * uCamera.view * model * local_position;

	vs_out.mvp_position = mvp_position;

	vs_out.depth = mvp_position.z / mvp_position.w;

	gl_Position = mvp_position;
}
