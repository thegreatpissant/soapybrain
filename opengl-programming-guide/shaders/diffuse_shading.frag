#version 330

in vec3 LightIntensity;

layout( location = 0 ) out vec4 FragColor;

void main(void)
{
    FragColor = vec4(LightIntensity , 1.0);
}
