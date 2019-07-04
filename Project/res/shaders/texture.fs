#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform vec3 textColor;//new var
uniform sampler2D texture1;
//uniform sampler2D texture2;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	//FragColor = mix(texture(texture1, TexCoord), texture(texture1, TexCoord), 0.2);
	FragColor = texture(texture1, TexCoord);
	//vec4 sampled = vec4(1.0, 1.0, 1.0, texture(texture1, TexCoord).r);
	//FragColor = vec4(textColor, 1.0)*sampled;
	//FragColor = vec4(0.0);
}