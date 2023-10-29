#version 330 core

uniform sampler2D tmp;

uniform float uFar;

uniform uint gDrawIndex;
uniform uint gObjectIndex;

out vec4 FragColor;

in VS_OUT {
	vec3 position;
	vec3 normal;
	vec2 texCoords;
} fs_in;


void main() {
	vec4 positionColor = vec4(fs_in.position.x, fs_in.position.y, fs_in.position.z, 1.0f);

	vec4 texColor = texture(tmp, fs_in.texCoords) + positionColor;
	FragColor = texColor;

	// FragColor = vec3(float(gObjectIndex), float(gDrawIndex), float(gl_PrimitiveID + 1));// + vec3(texColor);
}
