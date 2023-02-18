#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 mcolor;
out vec3 outcolor;

void main()
{
   gl_Position = vec4(position, 1.0f);
   outcolor = mcolor;
};

#shader fragment
#version 330 core

out vec4 color;
in  vec3 outcolor;
void main()
{
	color = vec4(outcolor, 1.0f);
};