#version 330 core
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Color;

out vec3 fragPosition;
out vec3 va_Color;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main()
{
    va_Color = a_Color;
    fragPosition = a_Pos;
    gl_Position = u_proj * u_view * u_model * vec4(a_Pos, 1.0);
};