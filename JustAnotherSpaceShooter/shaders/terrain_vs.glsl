#version 330

in vec3 pos_attrib;
in vec2 tex_coord_attrib;
in vec3 normal_attrib;

out vec2 o_tex_coo_attrib;

uniform mat4 terrainStart;
uniform mat4 t_view;
uniform mat4 t_projection;

void main() 
{
    o_tex_coo_attrib = tex_coord_attrib;
    gl_Position = t_projection * t_view * terrainStart * vec4(pos_attrib, 1.0);
}