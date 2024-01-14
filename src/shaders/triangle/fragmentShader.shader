#version 330 core
layout(location = 0) out vec4 outColor;

struct Camera {
	mat4 projection;
	mat4 view;

	float FOV;
	float width;
	float height;

	float near;
	float far;
};

struct Light {
	vec3 position;
	vec3 color;
};

struct MaterialTextures {
	sampler2D Ambient;
	sampler2D Diffuse;
	sampler2D Specular;
	sampler2D Specular_highlight;
	sampler2D Bump;
	sampler2D Displacement;
	sampler2D Alpha;
	sampler2D Reflection;
	sampler2D Roughness;
	sampler2D Metallic;
	sampler2D Sheen;
	sampler2D Emissive;
	sampler2D Normal;
};

struct MaterialCoefficients {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 transmittance;
	vec3 emission;

	float shininess;
	float ior;

	float roughness;
	float metallic;
	float sheen;
};

struct Material {
	MaterialTextures textures;
	MaterialCoefficients coefficients;
};

in VS_OUT {
	vec4 l_position;
	vec4 m_position;
	vec4 vm_position;
	vec4 mvp_position;

	vec4 lightPosition;

	vec2 texCoords;
	vec3 normal;
	float depth;

	// For flat shading
	// flat vec3 normal;
} fs_in;

uniform int uSelected;

uniform Camera uCamera;
uniform Light uLight;
uniform Material uMaterial;

vec3 calculateADS(float ambientStrength, float specularStrength, vec3 positionMV, vec3 normal, vec3 lightPosition, vec3 lightColor);

void main() {
	gl_FragDepth = fs_in.depth;

	vec3 normal        = normalize(fs_in.normal);
	vec3 positionMV    = fs_in.vm_position.xyz;
	vec3 lightPosition = fs_in.lightPosition.xyz;

	vec3 texDiffuse = texture(uMaterial.textures.Diffuse, fs_in.texCoords).xyz;

	vec3 ADS = calculateADS(0.3, 0.5, positionMV, normal, lightPosition, uLight.color);

	outColor = vec4(texDiffuse * ADS, 1.0);
}

vec3 calculateADS(float ambientStrength, float specularStrength, vec3 positionMV, vec3 normal, vec3 lightPosition, vec3 lightColor) {
	vec3 lightDir = normalize(lightPosition - positionMV);
	float diff    = max(dot(normal, lightDir), 0.0);

	vec3 viewDir    = normalize(-positionMV);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec      = pow(max(dot(viewDir, reflectDir), 0.0), 32);

	vec3 ambient  = ambientStrength         * lightColor * uMaterial.coefficients.ambient;
	vec3 diffuse  = diff                    * lightColor * uMaterial.coefficients.diffuse;
	vec3 specular = specularStrength * spec * lightColor * uMaterial.coefficients.specular;

	return (ambient + diffuse + specular);
}
