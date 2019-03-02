#extension GL_OES_standard_derivatives : enable

//precision highp float;

varying vec2 vUV;

uniform vec3 backgroundColor;
uniform vec3 gridColor;

uniform float divisions;
uniform float thickness;

void main(void) {

	float x = fract(vUV.x * divisions);
	x = min(x, 1.0 - x);

	float xdelta = fwidth(x);
	x = smoothstep(x - xdelta, x + xdelta, thickness);

	float y = fract(vUV.y * divisions);
	y = min(y, 1.0 - y);

	float ydelta = fwidth(y);
	y = smoothstep(y - ydelta, y + ydelta, thickness);

	float r = clamp(x + y, backgroundColor.x, gridColor.x);
	float g = clamp(x + y, backgroundColor.y, gridColor.y);
	float b = clamp(x + y, backgroundColor.z, gridColor.z);
	gl_FragColor = vec4(r, g, b, 1.0);
}

//#version 330 core
//#extension GL_OES_standard_derivatives : enable
//
//precision highp float;
//
//uniform int multiplicationFactor;
//uniform float threshold;
//uniform vec4 gridColor;
//uniform vec2 resolution;  // width and height of the viewport
//						  //uniform float minthres = 0.000002;
//						  //uniform float maxthres = 0.00005;
//in vec2 vUV;
//
//void main() {
//	//float m   = float(multiplicationFactor);
//	vec2 t = vUV * multiplicationFactor;
//	vec2 dist = abs(fract(t - 0.5) - 0.5) / resolution;
//	vec2 th = threshold / resolution;
//
//	//th = clamp(th, minthres, maxthres);
//
//	if (dist.x > th.x  && dist.y > th.y)
//		discard;
//	//gl_FragColor = vec4(vec3(1.0 - min(line, 1.0)), 1.0);
//	gl_FragColor = gridColor;
//}

