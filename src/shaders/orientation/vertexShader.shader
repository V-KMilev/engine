#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

out VS_OUT {
	vec4 local_position;
	vec4 position;

	vec2 texCoords;
	vec3 normal;
} vs_out;

void main() {
	vec4 local_position  = vec4(position, 1.0);
	vec4 position = uModel * mat4(mat3(uProjection * uView)) * local_position;

	vs_out.local_position  = local_position;
	vs_out.position = position;

	vs_out.texCoords = texCoords;
	vs_out. normal = normal;

	gl_Position = position;
}
