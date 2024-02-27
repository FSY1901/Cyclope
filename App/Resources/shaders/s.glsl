#type vertex

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform vec3 pos;
uniform vec3 camUp;
uniform vec3 camRight;
uniform mat4 viewProjection;

out vec2 TexCoords;

void main()
{
	vec3 res = pos + (camRight * aPos.x) + (camUp * -aPos.z);
	gl_Position = viewProjection * vec4(res, 1.0);
	TexCoords = aTexCoord;
}

#type fragment

#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D tex;
uniform vec3 diffuse;

void main()
{
	if(texture(tex, TexCoords).a < 0.1)
		discard;
	vec4 col = texture(tex, TexCoords) * vec4(diffuse, 1.0);
	FragColor = vec4(col);
}