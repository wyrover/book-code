#version 330

struct cameraState{
	mat4 perspective;
	mat4 view;
};

struct positionState{
	mat4 translation;
	mat4 rotation;
};

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;
layout (location = 2) in vec3 VertexNormal;

out VertexData {
	vec2 texCoord;
	vec3 normal;
	vec3 position;
} outData;

uniform cameraState camera;
uniform positionState object;

void main(){
	// model-view matrix
	mat4 objMatrix = (object.rotation * object.translation);
	// vertex position in the world
	//vec4 localPos = objMatrix * vec4(VertexPosition.xyz, 1.0);
	
	// final vertex position on screen
	gl_Position = (camera.perspective * inverse(camera.view)) * objMatrix * vec4(VertexPosition.xyz, 1.0);
	// texture coordinates and original vertex position
	// for the next stage - fragment shader
	outData.texCoord = vec2(VertexTexCoord.st);
	outData.position = VertexPosition.xyz;
	// normal vectors are adjusted to match object orientation
	vec4 tmpNormal = objMatrix * vec4(VertexNormal.xyz, 0.0);
	outData.normal = normalize(tmpNormal.xyz);
}
