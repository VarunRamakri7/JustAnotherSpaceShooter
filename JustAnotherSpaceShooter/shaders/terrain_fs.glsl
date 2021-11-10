#version 330

out vec4 final_color;

in vec3 var_pos_attrib;
in vec3 var_normal_attrib;

uniform mat4 terrainStart;
uniform vec3 light_direction;
uniform vec3 view_position;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform vec3 La;
uniform vec3 Ld;
uniform vec3 Ls;
uniform float shininess;

void main()
{
	vec3 ambient = Ka * La;

	vec3 nw = normalize(vec3(terrainStart * vec4(var_normal_attrib, 0.0)));
	vec3 pw = vec3(terrainStart * vec4(var_pos_attrib, 1.0));
	vec3 lw = normalize(-light_direction);
	float diff = max(dot(nw, lw), 0.0);
	vec3 diffuse = Kd * Ld * diff;

	vec3 vw = normalize(view_position - var_pos_attrib);
	vec3 rw = normalize(reflect(-lw, nw));
	float spec = pow(max(dot(vw, rw), 0.0), shininess);
	vec3 specular = Ks * Ls * spec;

	vec3 comb_color = ambient + diffuse + specular;
	final_color = vec4(comb_color, 1.0);
}
