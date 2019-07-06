#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform vec3 textColor;

// texture samplers
uniform sampler2D texture1;

void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(texture1, TexCoord).r);
	// vec4 tex = vec4(textColor, 1.0)*sampled;
	// if(tex.w < 0.1) 
	// 	discard;
	FragColor = vec4(textColor, 1.0)*sampled;
}