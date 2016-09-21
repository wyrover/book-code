#version 330 //use GLSL specification version 3.3
layout (location = 0) in vec3 VertexPosition;

uniform vec4 vertexColor;

out VertexData {
	vec4 Color;
} outData;

void main(){
	gl_Position = vec4(VertexPosition.xyz, 1.0);
	outData.Color = vec4(vertexColor.rgba);
}
