#version 150

in vec3 vertex_position;
in vec3 vertex_normal;

out vec3 light_intensity;

struct LightInfo {
	vec3 position;
	vec3 la;
	vec3 ld;
	vec3 ls;
};
uniform LightInfo light[8];
uniform int light_num;

struct MaterialInfo {
	vec3 ka;
	vec3 kd;
	vec3 ks;
	float shininess;
};
uniform MaterialInfo material;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;

void main()
{
	vec4 position = vec4(vertex_position, 1.0);
	mat4 view = inverse(camera);

	vec3 eye_norm = normalize(mat3(model) * vertex_normal);
	vec4 eye_position = view * model * position;

	for (int i = 0; i < light_num; i++) {
		vec3 s = normalize(vec3(light[i].position - eye_position.xyz));
		vec3 v = normalize(-eye_position.xyz);
		vec3 r = reflect(-s, eye_norm);
		vec3 ambient = light[i].la * material.ka;
		float sdotn = max(dot(s, eye_norm), 0.0);
		vec3 diffuse = light[i].ld * material.kd * sdotn;
		vec3 specular = vec3(0.0);
		if (sdotn > 0.0)
			specular = light[i].ls * material.ks *
				pow(max(dot(r, v), 0.0), material.shininess);

		light_intensity += (ambient + diffuse + specular);
	}

	gl_Position = projection * view * model * position;
}
