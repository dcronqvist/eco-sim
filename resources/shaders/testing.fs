#version 330 core
out vec4 FragColor;

in vec3 fragPosition;
in vec3 va_Color;

uniform float u_time;

void main()
{
    FragColor = vec4(0.5, 0.2, 0.3, 1.0);
};