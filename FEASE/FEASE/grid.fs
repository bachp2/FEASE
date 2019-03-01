#extension GL_OES_standard_derivatives : enable

precision highp float;

varying vec2 vUV;
uniform vec3 backgroundColor;
uniform vec3 gridColor;

uniform float divisions;
uniform float thickness;

void main(void) {
    float delta = 0.05 / 2.0;

    float x = fract(vUV.x * divisions);
    x = min(x, 1.0 - x);

    float xdelta = fwidth(x);
    x = smoothstep(x - xdelta, x + xdelta, thickness);

    float y = fract(vUV.y * divisions);
    y = min(y, 1.0 - y);

    float ydelta = fwidth(y);
    y = smoothstep(y - ydelta, y + ydelta, thickness);

    float r =clamp(x + y, backgroundColor.x, gridColor.x);
	float g =clamp(x + y, backgroundColor.y, gridColor.y);
	float b =clamp(x + y, backgroundColor.z, gridColor.z);
    gl_FragColor = vec4(r, g, b, 1.0);
}