#version 100

precision lowp float;
attribute vec4 vPosition;
attribute vec2 a_texCoord;
varying vec2 v_texCoord;

void main () {
	gl_Position = vPosition;
	v_texCoord = a_texCoord;
}
