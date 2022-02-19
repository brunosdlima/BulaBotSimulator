#shader vertex
#version 330 core //select the version of the API
layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP; //Model view project matrix

void main()
{
	gl_Position = u_MVP * position; //Give the positions of the current triangle vertex
	v_TexCoord = texCoord;
};


//************************************************************************************************************************


#shader fragmentation
#version 330 core //select the version of the API

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;
uniform vec4 u_color = { 0.0, 0.0, 1.0, 1.0 }; // Initialize the uniforme variable as blue

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = texColor; //Gives the color for te current body
};