#version 100

precision lowp float;

attribute vec4 vertexPosition;
attribute vec4 vertexColor;

varying vec4 fragmentColor;

void main () {
	gl_Position = vertexPosition;
    fragmentColor = vertexColor;
}