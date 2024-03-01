#type vertex

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 viewProjection;
uniform mat4 transform;

out vec2 TexCoords;

void main()
{
	gl_Position = viewProjection * transform* vec4(aPos, 1.0);
	TexCoords = aTexCoord;
}

#type fragment

#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

void main()
{
	FragColor = vec4(1.0, 0.2, 0.8, 1.0);
}