#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

mat4 get_cylindrical_billboard_view_matrix(){
	//float scalingFactor = sqrt(view[0][0]*view[0][0] + view[1][0]*view[1][0] + view[2][0]*view[2][0]);
	float scalingFactor = 1.0;
	vec4 c0 = vec4(scalingFactor*vec3(1.0, 0.0, 0.0), view[3][0]);
	vec4 c1 = vec4(view[1]);
	vec4 c2 = vec4(scalingFactor*vec3(0.0, 0.0, 1.0), view[3][2]);
	vec4 c3 = vec4(view[3]);
	return mat4(c0, c1, c2, c3);
}

mat4 get_spherical_billboard_view_matrix(){
	//float scalingFactor = sqrt(view[0][0]*view[0][0] + view[1][0]*view[1][0] + view[2][0]*view[2][0]);
	float scalingFactor = 1.0;
	vec4 c0 = vec4(scalingFactor*vec3(1.0, 0.0, 0.0), view[3][0]);
	vec4 c1 = vec4(scalingFactor*vec3(0.0, 1.0, 0.0), view[3][1]);
	vec4 c2 = vec4(scalingFactor*vec3(0.0, 0.0, 1.0), view[3][2]);
	vec4 c3 = vec4(view[3]);
	return mat4(c0, c1, c2, c3);
}

void main()
{	
	//gl_Position = projection * view  * model * vec4(aPos, 1.0f);
	
	mat4 billboard_view = get_spherical_billboard_view_matrix();
	gl_Position = projection * billboard_view  * model * vec4(aPos, 1.0f);
	
	//gl_Position = billboard_view  * model * vec4(aPos, 1.0f);
	//gl_Position = mvp * vec4(aPos, 1.0f);
	//gl_Position = vec4(aPos, 1.0f);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}