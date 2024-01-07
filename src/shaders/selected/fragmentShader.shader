#version 330 core
layout(location = 0) out vec4 outColor;

in VS_OUT {
	vec4 mvp_position;

	float depth;
} fs_in;

void main() {

	gl_FragDepth = fs_in.depth;

	outColor = vec4(1.0, 0.2, 0.0, 1.0);
}
