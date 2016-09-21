#version 330 //use GLSL specification version 3.3
layout (location = 0) in vec2 VertexPosition;
layout (location = 1) in vec4 VertexColor;
layout (location = 2) in vec2 VertexTexCoord;

out VertexData {
	vec4 Color;
	vec2 TexCoord;
} outData;

uniform mat3 modelViewMatrix;

void main(){
	vec3 newPos = modelViewMatrix * vec3(VertexPosition.xy, 1);
	gl_Position = vec4(newPos, 1.0);
	outData.Color = vec4(VertexColor.rgba);
	outData.TexCoord = vec2(VertexTexCoord.st);
}
