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
	//gl_Position = projection * view  * model * vec4(aPos, 1.0f);

	mat4 mv = view*model;
	
	// mat3 M1;
	// M1[0][0] = mv[0][0]; 
	// M1[0][1] = mv[0][1]; 
	// M1[0][2] = mv[0][2];

	// M1[1][0] = mv[1][0]; 
	// M1[1][1] = mv[1][1]; 
	// M1[1][2] = mv[1][2];

	// M1[2][0] = mv[2][0]; 
	// M1[2][1] = mv[2][1]; 
	// M1[2][2] = mv[2][2];

	mv[0][0] = 1.0; 
	mv[0][1] = 0.0; 
	mv[0][2] = 0.0;

	mv[1][0] = 0.0; 
	mv[1][1] = 1.0; 
	mv[1][2] = 0.0;

	mv[2][0] = 0.0; 
	mv[2][1] = 0.0; 
	mv[2][2] = 1.0;

	// vec3 obj_wcpos = vec3(0,0,1) + transpose(M1)*vec3(mv[3][0], mv[3][1], mv[3][2]);

	// mv[3][0] = obj_wcpos[0]; 
	// mv[3][1] = obj_wcpos[1]; 
	// mv[3][2] = obj_wcpos[2];

	//mv[3][0] = model[3][0]; 
	//mv[3][1] = model[3][1];
	//mv[3][2] = model[3][2];

	gl_Position = projection * mv * vec4(aPos, 1.0f);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}