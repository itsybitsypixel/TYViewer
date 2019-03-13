#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 skin;

out vec2 v_uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	v_uv = uv;
	gl_Position = projection * view * model * vec4(position, 1.0);
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_uv;

uniform sampler2D u_Texture;

void main()
{
	color = texture(u_Texture, v_uv);
};