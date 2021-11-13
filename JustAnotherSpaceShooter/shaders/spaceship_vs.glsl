#version 330

in vec3 pos_attrib;
in vec2 tex_coord_attrib;
in vec3 normal_attrib;

uniform mat4 model;
uniform mat4 s_view;
uniform mat4 s_projection;

out vec3 var_pos_attrib;
out vec3 var_normal_attrib;

void main() 
{
    var_pos_attrib = pos_attrib;
    var_normal_attrib = mat3(transpose(inverse(model))) * normal_attrib;

    gl_Position = s_projection * s_view * model * vec4(pos_attrib, 1.0);
}