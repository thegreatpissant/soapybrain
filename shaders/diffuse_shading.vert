#version 400
//  #version 330

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 VertexNormal;

// out float LightIntensity;

uniform vec4 LightPosition; //  Light position in eye coords
uniform float Kd;            //  Diffuse reflectivity
uniform float Ld;            //  Light source intensity

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
    //  Converte normal and position to eye coords
    /*
    vec3 tnorm = normalize( NormalMatrix * VertexNormal);
    vec4 eyeCoords = ModelViewMatrix * (vec4 (vPosition, 1.0));
    vec3 s = normalize (vec3 (LightPosition - eyeCoords));
    */
    //  The diffuse shading equation
    //  LightIntensity = Ld * Kd * max (dot (s, tnorm), 0.0 );
    /*
    LightIntensity = Ld * Kd * max ( 0.0, dot (normalize(LightPosition -eyeCoords), vec4(VertexNormal,1.0)));
    */
    //  Convert position to clip coordinates and pass along
    /*
    gl_Position = MVP * vec4 (vPosition, 1.0);
    */
    gl_Position = Projection * View * Model * vec4(vPosition, 1.0);
}
