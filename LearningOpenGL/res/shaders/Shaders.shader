#shader vertex

#version 330 core
layout(location = 0) in vec4 pos;     // the position variable has attribute position 0
layout(location = 1) in vec2 texCoord; // the color variable has attribute position 1

out vec2 aTexCoord;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * pos;
	aTexCoord = texCoord;
}
#shader fragment

#version 330 core
out vec4 FragColour;
in vec2 aTexCoord;

uniform sampler2D u_Texture;
uniform sampler2D u_Texture2;

void main()
{
	FragColour = vec4(mix( texture(u_Texture, aTexCoord), texture(u_Texture2, aTexCoord), 0.0));
}