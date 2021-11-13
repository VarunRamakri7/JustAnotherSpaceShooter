#version 330

out vec4 final_color;

in vec3 var_pos_attrib;
in vec3 var_normal_attrib;
in mat4 var_model;
in vec3 var_Ka;
in vec3 var_Kd;
in vec3 var_Ks;
in vec3 var_La;
in vec3 var_Ld;
in vec3 var_Ls;
in float var_shininess;

uniform vec3 light_direction;
uniform vec3 view_position;

void main()
{
	vec3 ambient = var_Ka * var_La;

	vec3 nw = normalize(vec3(var_model * vec4(var_normal_attrib, 0.0)));
	vec3 pw = vec3(var_model * vec4(var_pos_attrib, 1.0));
	vec3 lw = normalize(-light_direction);
	float diff = max(dot(nw, lw), 0.0);
	vec3 diffuse = var_Kd * var_Ld * diff;

	vec3 vw = normalize(view_position - var_pos_attrib);
	vec3 rw = normalize(reflect(-lw, nw));
	float spec = pow(max(dot(vw, rw), 0.0), var_shininess);
	vec3 specular = var_Ks * var_Ls * spec;

	vec3 comb_color = ambient + diffuse + specular;
	final_color = vec4(comb_color, 1.0);
}
