#version 330 core
layout (location = 0) in vec3 position;
layout (location = 0) in vec3 normal;
layout (location = 1) in vec2 texCoords;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

out VS_OUT {
	vec3 near;
	vec3 far;

	mat4 projection;
	mat4 view;
} vs_out;

vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 projection) {
	vec4 unprojectedPoint = inverse(view) * inverse(projection) * vec4(x, y, z, 1.0);

	return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() {
	vs_out.near = UnprojectPoint(position.x, position.y, 0.0, uView, uProjection);
	vs_out.far  = UnprojectPoint(position.x, position.y, 1.0, uView, uProjection);

	vs_out.projection = uProjection;
	vs_out.view       = uView;

	gl_Position = vec4(position, 1.0);
}
