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
	vec4 l_position;
	vec4 m_position;
	vec4 vm_position;
	vec4 mvp_position;

	vec2 texCoords;
	vec3 normal;
	float depth;

	// For flat shading
	// flat vec3 normal;
} vs_out;

void main() {
	vec4 l_position   = vec4(position, 1.0);
	vec4 m_position   = uModel * l_position;
	vec4 vm_position  = uCamera.view * m_position;
	vec4 mvp_position = uCamera.projection * vm_position;

	mat3 normal_matrix = mat3(transpose(inverse(uCamera.view * uModel)));

	vs_out.l_position   = l_position;
	vs_out.m_position   = m_position;
	vs_out.vm_position  = vm_position;
	vs_out.mvp_position = mvp_position;

	vs_out.texCoords = texCoords;
	vs_out.normal    = normal_matrix * normal;
	vs_out.depth     = mvp_position.z / mvp_position.w;

	gl_Position = mvp_position;
}
