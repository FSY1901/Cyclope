#type vertex

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 viewProjection;

void main()
{
   gl_Position = viewProjection * vec4(aPos, 1.0);
   TexCoord = aTexCoord;
}

#type fragment

#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D tex;

void main()
{
   FragColor = texture(tex, TexCoord);
}