#version 460 core
layout (location = 2) in vec3 aPos;
layout (location = 3) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    TexCoords = aTexCoords;
}