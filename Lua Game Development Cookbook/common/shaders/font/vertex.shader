#version 330 //use GLSL specification version 3.3
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;

out VertexData {
	vec2 TexCoord;
} outData;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 texMatrix;

void main(){
	vec4 newPos = projectionMatrix * modelViewMatrix * vec4(VertexPosition.xyz, 1);
	gl_Position = newPos;
	outData.TexCoord = (texMatrix * vec3(VertexTexCoord.st, 1)).st;
}
