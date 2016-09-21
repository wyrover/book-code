#version 330

in VertexData {
	vec4 Color;
	vec2 TexCoord;
} inData;

uniform sampler2D diffuseTexture;
uniform int textured;
uniform int useVertexColors;

layout(location = 0) out vec4 diffuseColor;

void main() {
	vec4 baseColor = vec4(1,1,1,1);

	if (useVertexColors>0) {
		baseColor = inData.Color;
	}

	if (textured>0){
		diffuseColor = baseColor * texture(diffuseTexture, inData.TexCoord.st);
	} else {
		diffuseColor = baseColor;
	}
}