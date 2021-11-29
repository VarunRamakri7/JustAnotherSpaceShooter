#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal; 
//layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec4 aModel[4];
layout (location = 7) in vec3 Ka;
layout (location = 8) in vec3 Kd;
layout (location = 9) in vec3 Ks;
layout (location = 10) in vec3 La;
layout (location = 11) in vec3 Ld;
layout (location = 12) in vec3 Ls;
layout (location = 13) in float shininess;

uniform mat4 view;
uniform mat4 projection;

out vec3 var_pos_attrib;
out vec3 var_normal_attrib;
// out vec2 var_tex_coord_attrib;
out mat4 var_model;
out vec3 var_Ka;
out vec3 var_Kd;
out vec3 var_Ks;
out vec3 var_La;
out vec3 var_Ld;
out vec3 var_Ls;
out float var_shininess;

void main()
{
	mat4 md;
	md[0] = aModel[0];
	md[1] = aModel[1];
	md[2] = aModel[2];
	md[3] = aModel[3];

	var_pos_attrib = aPos;
	// var_tex_coord_attrib = aTexCoord;
	var_model = md;
	var_Ka = Ka;
	var_Kd = Kd;
	var_Ks = Ks;
	var_La = La;
	var_Ld = Ld;
	var_Ls = Ls;
	var_shininess = shininess;

	var_normal_attrib = mat3(transpose(inverse(md))) * aNormal;
	gl_Position = projection * view * md * vec4(aPos, 1.0);
}
