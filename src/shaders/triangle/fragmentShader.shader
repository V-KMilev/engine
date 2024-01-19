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

uniform Camera uCamera;
uniform Light uLight;
uniform Material uMaterial;

in VS_OUT {
	vec4 l_position;
	vec4 m_position;
	vec4 vm_position;
	vec4 mvp_position;

	vec2 texCoords;
	vec3 normal;
	float depth;

	// For flat shading
	// flat vec3 normal;
} fs_in;

vec3 calculateADS(vec3 fragmentPosition, vec3 normal, vec3 lightPosition, vec3 lightColor, const float gamma);
// TODO
// vec3 calculatePBR();
// vec3 calculateRT();
// vec3 calculateNVRTX();

void main() {
	const float gamma = 2.2;

	gl_FragDepth = fs_in.depth;

	vec3 normal        = normalize(fs_in.normal);
	vec4 lightPosition = uCamera.view * vec4(uLight.position, 1.0);

	vec3 ADS = calculateADS(fs_in.vm_position.xyz, normal, lightPosition.xyz, uLight.color, gamma);

	// Gamma correction
	ADS = pow(ADS, vec3(1.0 / gamma));

	outColor = vec4(ADS, 1.0);
}

vec3 calculateADS(vec3 fragmentPosition, vec3 normal, vec3 lightPosition, vec3 lightColor, const float gamma) {
	vec3 lightDir   = normalize(lightPosition - fragmentPosition);
	vec3 viewDir    = normalize(uCamera.position - fragmentPosition);
	vec3 halfwayDir = normalize(lightDir + viewDir);

	float distance = length(lightPosition - fragmentPosition);
	float attenuation = 1.0 / (distance * distance);

	float diff = max(dot(lightDir, normal), 0.0);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), uMaterial.coefficients.shininess);

	vec3 ambient  = 0.3        * lightColor;
	vec3 diffuse  =       diff * lightColor;
	vec3 specular = 0.5 * spec * lightColor;

	// Gamma correct the textures
	ambient  *= uMaterial.coefficients.ambient  * pow(texture(uMaterial.textures.Ambient, fs_in.texCoords).rgb, vec3(gamma));
	diffuse  *= uMaterial.coefficients.diffuse  * pow(texture(uMaterial.textures.Diffuse, fs_in.texCoords).rgb, vec3(gamma));
	specular *= uMaterial.coefficients.specular * pow(texture(uMaterial.textures.Specular, fs_in.texCoords).rgb, vec3(gamma));

	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}
