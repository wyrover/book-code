#version 330

in VertexData {
	vec4 Color;
	vec2 TexCoord;
} inData;

uniform sampler2D diffuseTexture;
uniform int textured;

layout(location = 0) out vec4 diffuseColor;

void main() {
	if (textured>0){
		diffuseColor = inData.Color * texture(diffuseTexture, inData.TexCoord.st);
	} else {
		diffuseColor = inData.Color;
	}
}