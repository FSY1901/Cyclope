#type vertex

#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 viewProjection;
uniform mat4 transform;

out vec2 TexCoords;

void main()
{
	gl_Position = viewProjection* transform * vec4(aPos + aNormal*0.05, 1.0);
	TexCoords = aTexCoord;
}

#type fragment

#version 330 core
layout(location = 0) out vec4 color;
layout(location = 1) out int color2;

in vec2 TexCoords;

uniform int entityID;

void main()
{
	color = vec4(1.0, 0.5, 0.0, 1.0);
	color2 = entityID;
}