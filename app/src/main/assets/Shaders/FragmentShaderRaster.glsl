#version 100

precision lowp float;

varying vec4 fragmentPosition;
varying vec4 fragmentColor;

void main () {
	//gl_FragColor = fragmentColor;
    if (fragmentPosition.x >= 0.0) {
        gl_FragColor = vec4(0, 1, 0, 1);
    } else {
        gl_FragColor = vec4(0, 0, 0, 1);
    }
    gl_FragColor = fragmentPosition;
    gl_FragColor = vec4(0, 1, 0, 1);
}
