#version 330 core

uniform int multiplicationFactor;
uniform float threshold;
uniform vec4 gridColor;
 
in vec2 vUV;
 
void main() {
    // multiplicationFactor scales the number of stripes
	vec2 t = vUV * multiplicationFactor;

	// the threshold constant defines the with of the lines
	if (abs(t.x - round(t.x)) <= threshold  || abs(t.y - round(t.y)) < threshold )
		gl_FragColor = gridColor;    
	else
		discard;
}