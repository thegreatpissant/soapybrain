#version 150

#define _FRAGCOLOR_DECLARATION out vec4 FragColor;
#define _VS_IN in
#define _VS_OUT out
#define _FS_IN in
#define _TEXTURELOD textureLod
#define _FRAGCOLOR FragColor

uniform vec2 EyeToSourceUVScale;
uniform vec2 EyeToSourceUVOffset;

_VS_IN vec2 Position;
_VS_IN vec4 Color;
_VS_IN vec2 TexCoord0;
_VS_IN vec2 TexCoord1;
_VS_IN vec2 TexCoord2;

_VS_OUT vec4 oColor;
_VS_OUT vec2 oTexCoord0;
_VS_OUT vec2 oTexCoord1;
_VS_OUT vec2 oTexCoord2;

void main()
{
   gl_Position.x = Position.x;
   gl_Position.y = Position.y;
   gl_Position.z = 0.5;
   gl_Position.w = 1.0;
// Vertex inputs are in TanEyeAngle space for the R,G,B channels (i.e. after chromatic aberration and distortion).
// Scale them into the correct [0-1],[0-1] UV lookup space (depending on eye)
   oTexCoord0 = TexCoord0 * EyeToSourceUVScale + EyeToSourceUVOffset;
   oTexCoord0.y = 1.0 - oTexCoord0.y;
    oTexCoord1 = TexCoord1 * EyeToSourceUVScale + EyeToSourceUVOffset;
   oTexCoord1.y = 1.0-oTexCoord1.y;
   oTexCoord2 = TexCoord2 * EyeToSourceUVScale + EyeToSourceUVOffset;
   oTexCoord2.y = 1.0-oTexCoord2.y;

   oColor = Color;              // Used for vignette fade.
}








//in vec4 in_position;
//in vec2 in_tex_coord;

//out vec2 vs_tex_coord;

//void main (void)
//{

//    gl_Position = in_position;
//    vs_tex_coord = in_tex_coord;
////    vs_tex_coord = in_position.xy;
//}
