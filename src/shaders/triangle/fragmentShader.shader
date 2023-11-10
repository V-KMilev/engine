#version 330 core
layout(location = 0) out vec4 outColor;

uniform sampler2D tmp;

uniform int uSelected;

uniform float uNear;
uniform float uFar;

in VS_OUT {
	vec4 world_position;
	vec4 position;

	vec2 texCoords;
	vec3 normal;

	float depth;

	mat4 projection;
	mat4 view;
} fs_in;

void main() {
	gl_FragDepth = fs_in.depth;

	vec4 positionColor = vec4(fs_in.world_position.x, fs_in.world_position.y, fs_in.world_position.z, 1.0);

	outColor = texture(tmp, fs_in.texCoords) + vec4(positionColor.xyz, 1.0);

	if(bool(uSelected)) {
		outColor = vec4(1.0, 0.0, 0.0, 1.0);
	}
}
