#version 400


layout( location = 0 ) out vec4 FragColor;
layout( location = 1 ) in vec4 LightIntensity;

void main(void)
{
    //  FragColor = vec3(0.4, 0.4, 0.0 ) * LightIntensity; //  , 1.0);
    FragColor = LightIntensity;
}
