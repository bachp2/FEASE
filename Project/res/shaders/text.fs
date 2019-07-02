#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform vec3 textColor;//new var
// texture samplers
uniform sampler2D texture1;
//uniform sampler2D texture2;

void main()
{
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(texture1, TexCoord).r);
	FragColor = vec4(textColor, 1.0)*sampled;
}