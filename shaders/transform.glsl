#version 330

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 texcoord;

out EyeSpaceVertex {
	vec4 position;
	vec4 normal;
	vec2 texcoord;
} es_Out;

void main()
{
	es_Out.position = view * model * vec4(position, 1.0);
	es_Out.normal = view * model * vec4(normal, 0.0);
	es_Out.texcoord = texcoord.xy;
	es_Out.texcoord.y = -es_Out.texcoord.y;
	gl_Position = projection * es_Out.position;
}
