#version 330 core

uniform int multiplicationFactor;
uniform lowp float threshold;
uniform vec4 gridColor;
uniform vec2 resolution;  // width and height of the viewport
in vec2 vUV;
 
void main() {
    // multiplicationFactor scales the number of stripes
	float m   = float(multiplicationFactor);
    vec2 t    = vUV * m;
    vec2 dist = abs(t - round(t)) / m; 
    vec2 th   = threshold / resolution;
	// the threshold constant defines the with of the lines
	if (abs(t.x - round(t.x)) <= threshold  || abs(t.y - round(t.y)) < threshold )
		gl_FragColor = gridColor;    
	else
		discard;
}