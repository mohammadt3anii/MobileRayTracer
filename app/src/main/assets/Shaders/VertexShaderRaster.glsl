#version 100

precision lowp float;

uniform mat4 uniformModelMatrix;
uniform mat4 uniformViewMatrix;
uniform mat4 uniformProjectionMatrix;
uniform mat4 uniformMVPMatrix;

attribute vec4 vertexPosition;
attribute vec4 vertexColor;

varying vec4 fragmentColor;

void main () {
	//gl_Position = uniformMVPMatrix * vertexPosition;
	gl_Position = uniformProjectionMatrix * uniformViewMatrix * uniformModelMatrix * vertexPosition;
    fragmentColor = vertexColor;

    //fragmentColor = uniformMVPMatrix[1];
    //fragmentColor = vertexPosition;
}
