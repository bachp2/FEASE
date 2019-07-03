#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{	
	//gl_Position = projection * view  * model * vec4(aPos, 1.0f);

	mat4 mv = view*model;
	mv[0][0] = 1.0; 
	mv[0][1] = 0.0; 
	mv[0][2] = 0.0;

	mv[1][0] = 0.0; 
	mv[1][1] = 1.0; 
	mv[1][2] = 0.0;

	mv[2][0] = 0.0; 
	mv[2][1] = 0.0; 
	mv[2][2] = 1.0;

	gl_Position = projection * mv * vec4(aPos, 1.0f);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}