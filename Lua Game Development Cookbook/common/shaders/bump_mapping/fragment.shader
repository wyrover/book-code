#version 330

// a structure for light parameters
struct lightState {
	vec4 position;
	vec4 diffuse;
	vec4 specular;
	float constantAttenuation, linearAttenuation, quadraticAttenuation;
	float spotCutoff, spotExponent;
	vec3 spotDirection;
};

// structure with material properties
struct materialState {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

// camera position and orientation matrices
struct cameraState{
	mat4 perspective;
	mat4 view;
};

// diffuseTexture contains texture unit identifier (integer)
uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform cameraState camera;
uniform materialState material;

// ambient light color
uniform vec4 sceneAmbient;

//total number of lights, currently 8 is the maximum
uniform int totalLights; 
uniform lightState lights[8];

uniform int debug; 
uniform vec4 debugColor;
uniform vec3 normalScale;

in VertexData {
	vec2 texCoord;
	vec3 position;
	mat3 tbn;
} inData;

layout(location = 0) out vec4 diffuseColor;

/*
	Input:
		material - material type specification
		surface - voxel position in world space
		normalDirection - normal vector for current voxel
*/

vec4 processLighting(in materialState material, in vec3 surface, in vec3 normalDirection){
	// camera position in world space
	vec4 cam = camera.view * vec4(0,0,0,1);
	
	// directional vector from the surface to the camera
	// it's used primarily to determine highlights
	vec3 camDirection = normalize(cam.xyz - surface);
	vec3 lightDirection;

	float attenuation;

	// ambient light
	vec3 ambientLighting = sceneAmbient.rgb * material.ambient.rgb;
	vec3 totalLighting = ambientLighting;

	// iterate over all lights on the scene
	for (int index=0; index < totalLights; index++){
		lightState light = lights[index];
    
		// omni-directional ligh
		if (light.position.w == 0.0){
			lightDirection = light.position.xyz;
			attenuation = 1.0;
		}else{
			// directional light
			vec3 lightVector = light.position.xyz - surface.xyz;
			lightDirection = normalize(lightVector);
			float distance = length(lightVector);
      
			attenuation = 1.0 / (
				light.constantAttenuation +
				light.linearAttenuation * distance +
				light.quadraticAttenuation * (distance * distance)
			);

			/* spot-light
				Note: cut-off angle presents one half of
				light cone spatial angle
				A light with cut-off angle greater than 90 degrees
				is considered to be omni-light
			*/
			if (light.spotCutoff <= 90.0){
				float spotAngle = max(0.0, dot(lightDirection, light.spotDirection));
				
				// there's no light outside of light cone
				if (spotAngle < cos(radians(light.spotCutoff))){
					attenuation = 0.0;
				}else{
					attenuation *= pow(spotAngle, light.spotExponent);
				}
			}
		}

		/* 
			Diffuse light is dependent only on the surface normal
			and light direction
		*/
		vec3 diffuseReflection = attenuation * 
			light.diffuse.rgb * material.diffuse.rgb *
			max(0.0, dot(normalDirection, lightDirection));

		/*
			Specular reflection is present only if the light ray 
			reflects almost directly to camera lenses.
		*/
		vec3 specularReflection;
		
		// There's no specular reflection on the dark side
		if (dot(normalDirection, lightDirection) < 0.0) {
			specularReflection = vec3(0.0, 0.0, 0.0);
		} else {
		// Specular reflection
			specularReflection = attenuation *
				light.specular.rgb * material.specular.rgb *
				pow(
					max(0.0,
						dot(reflect(-lightDirection, normalDirection), camDirection)
					),
					material.shininess
				);
		}
    
		// Add to total lighting contribution
		totalLighting += diffuseReflection + specularReflection;
	}
	/*
		Material transparency is controlled by alpha channel
		of diffuse color
	*/
	return vec4(totalLighting, material.diffuse.a);
}

void main() {
	// texture texel
	vec4 texel = texture(diffuseTexture, inData.texCoord.st);
	// normalmap texel
	vec3 normalTexel = texture(normalTexture, inData.texCoord.st).xyz;

	materialState localMaterial = material;

	// Texel color is directly applied to current diffuse color
	localMaterial.diffuse *= texel;

	if (debug>0){
		diffuseColor = debugColor;
	}else{
		//normalize range
		normalTexel = normalize(2*normalTexel.xyz - vec3(1.0));

		//change normal vector orientation (depends on normal map texture)
		normalTexel *= normalScale;

		//convert normal map vector into world space
		vec3 perTexelNormal = inData.tbn * normalize(normalTexel);
		// Compute output color for current voxel
		
		diffuseColor = processLighting(
			localMaterial,
			inData.position,
			normalize(perTexelNormal)
		);
		
		
	}
}
