#version 150

#define _FRAGCOLOR_DECLARATION out vec4 FragColor;
#define _VS_IN in
#define _VS_OUT out
#define _FS_IN in
#define _TEXTURELOD textureLod
#define _FRAGCOLOR FragColor

uniform sampler2D Texture0;

_FS_IN vec4 oColor;
_FS_IN vec2 oTexCoord0;
_FS_IN vec2 oTexCoord1;
_FS_IN vec2 oTexCoord2;

_FRAGCOLOR_DECLARATION

void main()
{
   float ResultR = _TEXTURELOD(Texture0, oTexCoord0, 0.0).r;
   float ResultG = _TEXTURELOD(Texture0, oTexCoord1, 0.0).g;
   float ResultB = _TEXTURELOD(Texture0, oTexCoord2, 0.0).b;

   _FRAGCOLOR = vec4(ResultR * oColor.r, ResultG * oColor.g, ResultB * oColor.b, 1.0);
}

//uniform sampler2D tex;

//in vec2 vs_tex_coord;

//out vec4 color;

//void main () {
//    color = texture( tex, vs_tex_coord);
//}
