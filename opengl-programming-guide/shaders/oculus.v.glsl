#version 330

in vec4 in_position;
in vec2 in_tex_coord;

out vec2 vs_tex_coord;

void main (void)
{
    gl_Position = in_position;
    vs_tex_coord = in_tex_coord;
}
