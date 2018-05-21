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
	gl_Position = uniformProjectionMatrix * uniformViewMatrix * uniformModelMatrix * vertexPosition;
	gl_Position = uniformMVPMatrix * vertexPosition;
    fragmentColor = vertexColor;
}
