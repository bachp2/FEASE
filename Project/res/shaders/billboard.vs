#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

void main()
{
	//mat4 mv = view;
	//float scale = sqrt(mv[0][0]*mv[0][0] + mv[1][0]*mv[1][0] + mv[2][0]*mv[2][0]);
	// mv[0][0] = 1; 
	// mv[1][0] = 0.0; 
	// mv[2][0] = 0.0;

	// mv[0][1] = 0.0; 
	// mv[1][1] = 0.05; 
	// mv[2][1] = 0.0;

	// mv[0][2] = 0.0; 
	// mv[1][2] = 0.0; 
	// mv[2][2] = 1;

	// mv[3][0] += 0.1; 
	// mv[3][1] += 0; 
	// mv[3][2] += 0.1;
	//gl_Position = projection*mv*model*vec4(aPos, 1.0f);
	gl_Position = projection*view*model*vec4(aPos, 1.0f);
}
