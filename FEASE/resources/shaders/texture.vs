#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{	
	vec4 c0 = vec4(1.0, 0.0, 0.0, view[3][0]);
	vec4 c1 = vec4(0.0, 1.0, 0.0, view[3][1]);
	//vec4 c1 = vec4(view[1]);
	vec4 c2 = vec4(0.0, 0.0, 1.0, view[3][2]);
	vec4 c3 = vec4(view[3]);
	mat4 billboard_view = mat4(c0, c1, c2, c3);
	gl_Position = projection * billboard_view  * model * vec4(aPos, 1.0f);
	//gl_Position = mvp * vec4(aPos, 1.0f);
	//gl_Position = projection * vec4(aPos, 1.0f);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}