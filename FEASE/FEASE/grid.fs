#version 330 core
#extension GL_OES_standard_derivatives : enable

precision highp float;

uniform float divisions;
uniform float thickness;
uniform vec3 gridColor;
uniform vec2 resolution;  // width and height of the viewport

in vec2 vUV;

void main() {
	vec2 t = vUV * divisions;
	vec2 dist = abs(fract(t - 0.5) - 0.5) / resolution;
	vec2 th = thickness / resolution;

	if (dist.x > th.x  && dist.y > th.y)
		discard;
	gl_FragColor = vec4(gridColor, 1.0);
}

