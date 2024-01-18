#version 330 core
layout(location = 0) out vec3 outColor;

uniform uint uObjectID;

void main() {
	outColor = vec3(float(uObjectID), float(gl_PrimitiveID), 0.0);
}
