#version 330

struct cameraState{
	mat4 perspective;
	mat4 view;
};

struct positionState{
	mat4 view;
};

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;
layout (location = 2) in vec3 VertexNormal;
layout (location = 3) in vec4 VertexTangent;

out VertexData {
	vec2 texCoord;
	vec3 position;
	mat3 tbn;
} outData;

uniform cameraState camera;
uniform positionState object;

void main(){
	// model-view matrix
	mat4 objMatrix = object.view;
	vec4 vertexPos = objMatrix * vec4(VertexPosition.xyz, 1.0);

	// final vertex position on screen
	// don't forget to use inverse of the camera.view matrix to move whole scene!
	gl_Position = (camera.perspective * inverse(camera.view)) * vertexPos;
	
	// texture coordinates and original vertex position
	// for the next stage - fragment shader
	outData.texCoord = vec2(VertexTexCoord.st);
	outData.position = vertexPos.xyz;

	outData.tbn = mat3(
		normalize((objMatrix * vec4(VertexTangent.xyz, 0.0)).xyz),
		normalize((objMatrix * vec4(cross(VertexNormal, VertexTangent.xyz) * VertexTangent.w, 0.0)).xyz),
		normalize(objMatrix * vec4(VertexNormal.xyz, 0.0)).xyz
	);
}
