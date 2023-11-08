#version 330 core
layout (location = 0) in vec3 position;
layout (location = 0) in vec3 normal;
layout (location = 1) in vec2 texCoords;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

out VS_OUT {
	vec4 world_position;
	vec4 position;

	vec2 texCoords;
	vec3 normal;

	float depth;

	mat4 projection;
	mat4 view;
} vs_out;

void main() {
	vec4 world_position = uProjection * uView * uModel * vec4(position, 1.0);

	vs_out.position       = uModel * vec4(position, 1.0);
	vs_out.world_position = world_position;

	vs_out.texCoords = texCoords;

	vs_out.depth = world_position.z / world_position.w;

	vs_out.projection = uProjection;
	vs_out.view       = uView;

	gl_Position = world_position;
}
