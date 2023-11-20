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

out VS_OUT {
	vec4 local_position;
	vec4 position;

	vec2 texCoords;
	vec3 normal;
} vs_out;

void main() {
	vec4 local_position  = vec4(position, 1.0);
	vec4 position = uModel * mat4(mat3(uCamera.projection * uCamera.view)) * local_position;

	vs_out.local_position  = local_position;
	vs_out.position = position;

	vs_out.texCoords = texCoords;
	vs_out. normal = normal;

	gl_Position = position;
}
