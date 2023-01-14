#version 400

in float LightIntensity;

layout( location = 0 ) out vec3 FragColor;

void main(void)
{
    FragColor = vec3(0.4, 0.4, 0.0 ) * LightIntensity; //  , 1.0);
}
