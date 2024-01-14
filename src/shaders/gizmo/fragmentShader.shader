#version 330 core
layout(location = 0) out vec4 outColor;

in VS_OUT {
	vec4 local_position;
	vec4 world_position;
	vec4 mvp_position;

	vec2 texCoords;
	vec3 normal;

	float depth;
} fs_in;

void main() {
	gl_FragDepth = 0.9;

	vec3 normal = normalize(fs_in.normal);

	vec3 color;

	if (abs(normal.x) > abs(normal.y) && abs(normal.x) > abs(normal.z)) {
		// Red for faces perpendicular to the X-axis
		color = vec3(1.0, 0.0, 0.0);
	} else if (abs(normal.y) > abs(normal.z)) {
		// Green for faces perpendicular to the Y-axis
		color = vec3(0.0, 1.0, 0.0);
	} else {
		// Blue for faces perpendicular to the Z-axis
		color = vec3(0.0, 0.0, 1.0);
	}

	outColor = vec4(color, 0.5);
}
