#version 120

in vec3 light_intensity;

void main()
{
	gl_FragColor = vec4(vec3(light_intensity), 1.0);
}
