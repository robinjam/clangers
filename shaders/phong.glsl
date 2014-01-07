#version 330

uniform sampler2D diffuse_map;
uniform sampler2D specular_map;
uniform samplerCube cubemap;

uniform mat4 view;

in EyeSpaceVertex {
	vec4 position;
	vec4 normal;
	vec2 texcoord;
} es_In;

out vec4 colour;

const vec3 light_diffuse = vec3(1.0, 1.0, 1.0);
const vec3 light_specular = vec3(1.0, 1.0, 1.0);

void main()
{
	vec4 light_direction = view * vec4(-1.0, 0.0, 1.0, 0.0);
	vec4 light_reflect_direction = normalize(reflect(-light_direction, es_In.normal));
	vec4 camera_direction = vec4(0.0, 0.0, 1.0, 0.0);

	vec3 material_diffuse = texture(diffuse_map, es_In.texcoord).xyz;
	vec4 material_specular = texture(specular_map, es_In.texcoord);

	vec3 diffuse = material_diffuse * max(dot(light_direction, es_In.normal), 0.0) * light_diffuse;
	vec3 specular = max(material_specular.xyz * pow(max(dot(light_reflect_direction, camera_direction), 0.0), material_specular.w * 128) * light_specular, 0.0);

	vec3 reflected_direction = reflect(camera_direction, es_In.normal).xyz;
	colour = vec4(diffuse + specular, 1.0) + material_specular.w * texture(cubemap, reflected_direction);
}
