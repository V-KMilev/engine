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
	vec3 near;
	vec3 far;
} vs_out;

vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 projection) {
	vec4 unprojectedPoint = inverse(view) * inverse(projection) * vec4(x, y, z, 1.0);

	return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() {
	vs_out.near = UnprojectPoint(position.x, position.y, 0.0, uCamera.view, uCamera.projection);
	vs_out.far  = UnprojectPoint(position.x, position.y, 1.0, uCamera.view, uCamera.projection);

	gl_Position = vec4(position, 1.0);
}
