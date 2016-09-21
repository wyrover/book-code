#version 330 //use GLSL specification version 3.3
layout (location = 0) in vec2 VertexPosition;
layout (location = 1) in vec4 VertexColor;
layout (location = 2) in vec2 VertexTexCoord;

out VertexData {
	vec4 Color;
	vec2 TexCoord;
} outData;

void main(){
	gl_Position = vec4(VertexPosition.xy, 0.0, 1.0);
	outData.Color = vec4(VertexColor.rgba);
	outData.TexCoord = vec2(VertexTexCoord.st);
}
