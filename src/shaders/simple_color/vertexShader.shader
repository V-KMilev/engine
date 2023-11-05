#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;

void main() {
	gl_Position = uProjection * uView * uModel * vec4(position, 1.0);
}
