#version 330

in VertexData {
	vec2 TexCoord;
} inData;

uniform sampler2D diffuseTexture;
uniform vec4 baseColor;

layout(location = 0) out vec4 diffuseColor;

void main() {
	diffuseColor = baseColor * texelFetch(diffuseTexture, ivec2(inData.TexCoord.st), 0);
}