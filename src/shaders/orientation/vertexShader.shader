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
	vec4 local_position;
	vec4 position;

	vec2 texCoords;
	vec3 normal;
} vs_out;

mat4 fixedFovProjection(mat4 projection, float fov, float width, float height) {
	mat4 viewProjection = projection;

	float aspect = width / height;

	float tanHalfFov = tan(radians(fov / 2.0));

	viewProjection[0][0] = 1.0 / (aspect * tanHalfFov);
	viewProjection[1][1] = 1.0 / (tanHalfFov);

	return viewProjection;
}

void main() {
	mat4 projection = fixedFovProjection(uCamera.projection, 45.0, uCamera.width, uCamera.height);

	vec4 local_position  = vec4(position, 1.0);
	vec4 position = uModel * mat4(mat3(projection * uCamera.view)) * local_position;

	vs_out.local_position  = local_position;
	vs_out.position = position;

	vs_out.texCoords = texCoords;
	vs_out. normal = normal;

	gl_Position = position;
}
