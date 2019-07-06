#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform sampler2D texture1;

void main()
{
	// mat4 mv = projection * view * model;
	// mv[0][0] = 3.0; 
	// mv[1][0] = 0.0; 
	// mv[2][0] = 0.0;

	// mv[0][1] = 0.0; 
	// mv[1][1] = 3.0; 
	// mv[2][1] = 0.0;

	// mv[0][2] = 0.0; 
	// mv[1][2] = 0.0; 
	// mv[2][2] = 3.0;

	// mv[3][0] += 0.1; 
	// mv[3][1] += 0; 
	// mv[3][2] += 0.1;

	gl_Position = projection * view * model * vec4(aPos, 1.0f);

	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}