#version 330

in vec3 pos_attrib;
in vec2 tex_coord_attrib;
in vec3 normal_attrib;

uniform mat4 terrainStart;
uniform mat4 t_view;
uniform mat4 t_projection;

out vec3 var_pos_attrib;
out vec3 var_normal_attrib;

void main() 
{
    var_pos_attrib = pos_attrib;
    var_normal_attrib = mat3(transpose(inverse(terrainStart))) * normal_attrib;

    gl_Position = t_projection * t_view * terrainStart * vec4(pos_attrib, 1.0);
}