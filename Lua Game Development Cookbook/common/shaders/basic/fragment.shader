#version 330

in VertexData {
	vec4 Color;
} inData;

layout(location = 0) out vec4 diffuseColor;

void main() {
	diffuseColor = inData.Color;
}