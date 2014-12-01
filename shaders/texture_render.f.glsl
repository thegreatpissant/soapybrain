#version 330

uniform sampler2D tex;

in vec2 vs_tex_coord;

out vec4 color;

void main () {
    color = texture( tex, vs_tex_coord);
}
