#shader vertex

#version 330 core
layout(location = 0) in vec4 pos;     // the position variable has attribute position 0

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * pos;
}

#shader fragment

#version 330 core
out vec4 FragColour;

void main()
{
	FragColour = vec4(1.0, 0.0, 0.0, 1.0f);
}