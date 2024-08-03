#version 330 core
layout (location = 0) in vec3 a_pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out float height;

void main()
{
	gl_Position = projection * view * model * vec4(a_pos, 1.0);
	height = a_pos.y;
}