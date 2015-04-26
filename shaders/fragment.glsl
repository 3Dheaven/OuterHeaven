#version 410 core

in vec3 V;
in vec3 L;
in vec3 N;

uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;

layout(location = 0) out vec3 color;
 
void main()
{
	
	vec3 Vn = normalize(V);
	vec3 Ln = normalize(L);
	vec3 Nn = normalize(N);
	vec3 R = reflect(-Ln,Nn);

	vec3 Ia = vec3(0.4, 0.4, 0.4);
	vec3 Id = vec3(0.5, 0.5, 0.5);
	vec3 Is = vec3(1.0, 1.0, 1.0);
       
	vec3 a = ka * Ia;
	vec3 d = max(dot(Ln,Nn),0) * kd * Id;
	vec3 s = pow(max(dot(R,Vn),0),64) * ks * Is;

	color = a+d+s;
}