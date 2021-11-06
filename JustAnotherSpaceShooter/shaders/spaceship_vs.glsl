#version 330

in vec3 pos_attrib;
in vec2 tex_coord_attrib;
in vec3 normal_attrib;

out vec2 o_tex_coo_attrib;

uniform mat4 spaceship;
uniform mat4 s_view;
uniform mat4 s_projection;

void main() 
{
    o_tex_coo_attrib = tex_coord_attrib;
    gl_Position = s_projection * s_view * spaceship * vec4(pos_attrib, 1.0);
}