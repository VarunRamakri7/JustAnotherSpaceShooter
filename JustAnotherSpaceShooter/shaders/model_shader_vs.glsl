#version 330

in vec3 pos_attrib;
in vec2 tex_coord_attrib;
in vec3 normal_attrib;

out vec2 o_tex_coo_attrib;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() 
{
    o_tex_coo_attrib = tex_coord_attrib;
    gl_Position = projection * view * model * vec4(pos_attrib, 1.0);
}