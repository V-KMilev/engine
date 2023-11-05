#version 330 core
layout(location = 0) out vec4 outColor;

uniform float uNear;
uniform float uFar;

in VS_OUT {
	vec3 near;
	vec3 far;

	mat4 projection;
	mat4 view;
} fs_in;

vec4 grid(vec3 fragmentPosition3D, float scale, vec4 gridColor) {
	vec2 coord = fragmentPosition3D.xz * scale;

	vec2 derivative = fwidth(coord);

	vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;

	float line = min(grid.x, grid.y);

	float minimumz = min(derivative.y, 1);
	float minimumx = min(derivative.x, 1);

	vec4 color;

	if (line < 1.0) {
		color = gridColor;
	}
	else {
		discard;
	}

	if (abs(fragmentPosition3D.x) < 0.1 * minimumx) {
		color.r = 1.0;
	}
	if (abs(fragmentPosition3D.z) < 0.1 * minimumz) {
		color.b = 1.0;
	}

	return color;
}

float computeDepth(vec3 position) {
	vec4 clip_space_pos = fs_in.projection * fs_in.view * vec4(position, 1.0);
	return (clip_space_pos.z / clip_space_pos.w);
}

float LinearizeDepth(float depth, float near, float far) {
	// Back to NDC
	float z = depth * 2.0 - 1.0;
	return (2.0 * near * far) / (far + near - z * (far - near));
}

void main() {
	float t = -fs_in.near.y / (fs_in.far.y - fs_in.near.y);
	vec3 fragmentPosition3D = fs_in.near + t * (fs_in.far - fs_in.near);

	gl_FragDepth = computeDepth(fragmentPosition3D);

	float linearDepth = LinearizeDepth(gl_FragDepth, uNear, uFar) / 2;

	outColor = (
		grid(fragmentPosition3D, 100.0, vec4(0.2, 0.2, 0.2, 1.0)) +
		grid(fragmentPosition3D, 1.0  , vec4(0.2, 0.2, 0.2, 1.0))) * float(t > 0);

	outColor /= linearDepth;
}
