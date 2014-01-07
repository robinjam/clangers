#version 330

uniform samplerCube cubemap;

in vec3 texcoord;

out vec4 colour;

void main()
{
	colour = texture(cubemap, texcoord);
}
