#version 330

uniform mat4 view;
uniform mat4 projection;

out vec3 texcoord;

void main()
{
	gl_Position = vec4(
		(gl_VertexID == 0) ? 3.0 : -1.0,
		(gl_VertexID == 1) ? 3.0 : -1.0,
		0.0,
		1.0
	);

	mat4 r = view;
	r[3][0] = 0.0;
    r[3][1] = 0.0;
    r[3][2] = 0.0;

	texcoord = (inverse(r) * inverse(projection) * gl_Position).xyz;
}
