#version 330

uniform sampler2D tex;

in EyeSpaceVertex {
	vec4 position;
	vec4 normal;
	vec2 texcoord;
} es_In;

out vec4 colour;

const vec4 light_diffuse = vec4(1.0, 1.0, 1.0, 1.0);

void main()
{
	vec4 light_direction = vec4(1.0, 0.0, 1.0, 0.0);
	colour = texture(tex, es_In.texcoord) * max(dot(light_direction, es_In.normal), 0.0) * light_diffuse;
}
