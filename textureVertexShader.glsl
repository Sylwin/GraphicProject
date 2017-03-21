#version 330 core

layout (location = 0) in vec3 a_position;	//vertex position
layout (location = 1) in vec2 a_uv;			//attribute texture coordinates for our current pixel

out vec2 v_uv;		//interpolated texture coordinates for current pixel

void main()
{
    gl_Position = vec4(a_position, 1.0);
	v_uv = (a_position.xy*0.5) + vec2(0.5,0.5);
}
 