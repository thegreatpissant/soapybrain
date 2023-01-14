#version 400
//  #version 330

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 VertexNormal;

// out float LightIntensity;
out vec4 v_Color;
uniform vec3 LightPosition; //  Light position in eye coords
//  uniform float Kd;            //  Diffuse reflectivity
//  uniform float Ld;            //  Light source intensity

/*
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;
*/

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;



void main(void)
{
    vec3 modelViewVertex = vec3( View * Model * vec4(vPosition, 0.0));
    vec3 modelViewNormal = vec3( View * Model * vec4(VertexNormal, 0.0));
    float distance = length(LightPosition - modelViewVertex);
    vec3 lightVector = normalize(LightPosition - modelViewVertex);
    float diffuse = max(dot(modelViewNormal, lightVector), 1.0);
    diffuse = diffuse * (1.0 / (1.0 + (0.25 * distance * distance)));
    //  v_Color = a_Color * diffuse;
    v_Color = vec4(1.0) * diffuse;
    //  gl_Position = Projection * View * Model * vec4(vPosition, 1.0);
}
