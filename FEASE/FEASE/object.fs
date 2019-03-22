#version 330 core
out vec4 FragColor;
uniform vec3 dotColor;
void main()
{
    FragColor = vec4(dotColor, 1.0);
} 