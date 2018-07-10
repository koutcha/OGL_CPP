#version 330 core
layout (location = 0) in vec3 position;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform float scale;

void main()
{
    TexCoords = position;

	vec3 tempPos = position*scale;

    vec4 pos = projection * view * vec4(tempPos, 1.0);
    gl_Position = pos.xyww;
}  