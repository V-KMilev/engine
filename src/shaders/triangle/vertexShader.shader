#version 330 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec2 texCoords;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

out VS_OUT {
	vec3 position;
	vec3 normal;
	vec2 texCoords;
} vs_out;

void main() {
	vs_out.position = vec3(uModel * position);
	vs_out.texCoords = texCoords;

	gl_Position = uProjection * uView * uModel * position; //uProjection * uView * uModel * position;
}
