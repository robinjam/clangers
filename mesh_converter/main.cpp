#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/mesh.h>
#include <fstream>
#include <OpenGL/gl3.h>

int main(int argc, const char *argv[])
{
	if (argc != 3) {
		std::cout << "Usage: " << argv[0] << " input-file output-file" << std::endl;
		return 0;
	}

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(argv[1], aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);

	if (!scene) {
		std::cerr << "Error: Unable to load geometry from " << argv[1] << std::endl;
		return 1;
	}

	if (scene->mNumMeshes != 1) {
		std::cerr << "Error: Only scenes with exactly 1 mesh are supported" << std::endl;
		return 1;
	}

	aiMesh *ai_mesh = scene->mMeshes[0];
	std::ofstream out(argv[2]);

	std::cout << ai_mesh->mNumVertices << " verts" << std::endl;
	std::cout << ai_mesh->mNumFaces << " faces" << std::endl;

	uint32_t magic_number = 2811273200,
		vbo_size = sizeof(float) * 9 * ai_mesh->mNumVertices,
		vbo_usage = GL_STATIC_DRAW,
		ibo_size = sizeof(uint32_t) * 3 * ai_mesh->mNumFaces,
		ibo_usage = GL_STATIC_DRAW,
		num_indices = ai_mesh->mNumFaces * 3,
		num_attributes = 3,
		stride = num_attributes * sizeof(float) * 3;
	out.write((char *) &magic_number, sizeof(uint32_t));
	out.write((char *) &vbo_size, sizeof(uint32_t));
	out.write((char *) &vbo_usage, sizeof(uint32_t));
	out.write((char *) &ibo_size, sizeof(uint32_t));
	out.write((char *) &ibo_usage, sizeof(uint32_t));
	out.write((char *) &num_indices, sizeof(uint32_t));
	out.write((char *) &num_attributes, sizeof(uint32_t));
	for (uint32_t i = 0; i < 3; ++i) {
		int32_t attribute_size = 3;
		uint8_t normalized = GL_FALSE;
		uint32_t pointer = sizeof(float) * 3 * i;
		out.write((char *) &i, sizeof(uint32_t));
		out.write((char *) &attribute_size, sizeof(int32_t));
		out.write((char *) &normalized, sizeof(uint8_t));
		out.write((char *) &stride, sizeof(uint32_t));
		out.write((char *) &pointer, sizeof(uint32_t));
	}

	for (uint32_t i = 0; i < ai_mesh->mNumVertices; ++i) {
		const aiVector3D &p = ai_mesh->mVertices[i];
		const aiVector3D &n = ai_mesh->mNormals[i];
		const aiVector3D &t = ai_mesh->mTextureCoords[0][i];
		out.write((char *) &p.x, sizeof(float));
		out.write((char *) &p.y, sizeof(float));
		out.write((char *) &p.z, sizeof(float));
		out.write((char *) &n.x, sizeof(float));
		out.write((char *) &n.y, sizeof(float));
		out.write((char *) &n.z, sizeof(float));
		out.write((char *) &t.x, sizeof(float));
		out.write((char *) &t.y, sizeof(float));
		out.write((char *) &t.z, sizeof(float));
	}

	for (uint32_t i = 0; i < ai_mesh->mNumFaces; ++i) {
		const aiFace &face = ai_mesh->mFaces[i];
		uint32_t a = face.mIndices[0],
			b = face.mIndices[1],
			c = face.mIndices[2];
		out.write((char *) &a, sizeof(uint32_t));
		out.write((char *) &b, sizeof(uint32_t));
		out.write((char *) &c, sizeof(uint32_t));
	}
}
