#version 330 core
layout(location = 0) out vec4 outColor;

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

in VS_OUT {
	vec3 normal;
	float depth;
} fs_in;

void main() {
	const float gamma = 2.2;

	gl_FragDepth = fs_in.depth;
	vec3 normal = normalize(fs_in.normal);

	vec3 color;

	if (abs(normal.x) > abs(normal.y) && abs(normal.x) > abs(normal.z)) {
		// Blue for faces perpendicular to the X-axis
		color = vec3(1.0, 0.0, 0.0);
	} else if (abs(normal.y) > abs(normal.z)) {
		// Green for faces perpendicular to the Y-axis
		color = vec3(0.0, 1.0, 0.0);
	} else {
		// Red for faces perpendicular to the Z-axis
		color = vec3(0.0, 0.0, 1.0);
	}

	// Gamma correction
	color = pow(color, vec3(1.0 / gamma));

	outColor = vec4(color, 1.0);
}
