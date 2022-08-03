#include "assimpLoader.hpp"
#include "debug.hpp"

namespace assimp
{ 
void loadModel(std::string path, std::string filename, std::shared_ptr<engine::Model> model)
{
	// Load aiScene
	uint32_t flags = uint32_t(aiProcess_Triangulate | aiProcess_GenBoundingBoxes | aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace);

	Assimp::Importer importer;
	const aiScene* assimpScene = importer.ReadFile(path + filename, flags);
	ASSERT(assimpScene);

	uint32_t numMeshes = assimpScene->mNumMeshes;

	// Load vertex data
	model->name = filename;
	model->meshes.resize(numMeshes);

	ASSERT(sizeof(XMFLOAT3) == sizeof(aiVector3D));
	ASSERT(sizeof(engine::Mesh::Triangle) == 3 * sizeof(uint32_t));

	engine::Model::MeshRange range;
	range.vertexOffset = 0u;
	range.indexOffset = 0u;
	range.vertexNum = 0u;
	range.indexNum = 0u;


	std::vector<engine::Mesh::Vertex> vertices;
	std::vector<engine::Mesh::Triangle> indices;

	for (uint32_t i = 0; i < numMeshes; ++i)
	{
		range.vertexOffset += range.vertexNum;
		range.indexOffset += range.indexNum;

		auto& srcMesh = assimpScene->mMeshes[i];
		auto& srcMaterial = assimpScene->mMaterials[i];
		auto& dstMesh = model->meshes[i];

		range.vertexNum = srcMesh->mNumVertices;
		range.indexNum = srcMesh->mNumFaces * 3;

		model->addMeshRange(range);


		dstMesh.name = srcMesh->mName.C_Str();

		dstMesh.vertices.resize(srcMesh->mNumVertices);
		dstMesh.triangles.resize(srcMesh->mNumFaces);
		dstMesh.materials.resize(srcMaterial->GetTextureCount(aiTextureType_DIFFUSE));

		dstMesh.m_transform = XMMatrixScaling(1.0f, 1.0f, 1.0f);
		XMMATRIX x = reinterpret_cast<XMMATRIX&>(assimpScene->mRootNode->mChildren[0]->mTransformation);
		dstMesh.m_transform = x;

		if (assimpScene->HasMaterials())
		{
			aiMaterial* material = assimpScene->mMaterials[i];
			aiString name;
			assimpScene->mMaterials[i]->Get(AI_MATKEY_NAME, name);
			for (uint32_t k = 0; k != dstMesh.materials.size(); ++k)
			{
				srcMaterial->GetTexture(aiTextureType_DIFFUSE, k, &name);

				std::string str = path + name.C_Str();

				while(str.find("tga") != std::string::npos)
					str.replace(str.find("tga"), 3, "dds");
				while (str.find("TGA") != std::string::npos)
					str.replace(str.find("TGA"), 3, "dds");

				dstMesh.materials[k].texture = engine::TextureManager::GetInstance()->getTexture(str);
			}
		}

		for (uint32_t v = 0; v < srcMesh->mNumVertices; ++v)
		{
			engine::Mesh::Vertex& vertex = dstMesh.vertices[v];
			vertex.position = reinterpret_cast<XMFLOAT3&>(srcMesh->mVertices[v]);
			vertex.texCoords = reinterpret_cast<XMFLOAT2&>(srcMesh->mTextureCoords[0][v]);
			vertices.push_back(vertex);
		}

		for (uint32_t f = 0; f < srcMesh->mNumFaces; ++f)
		{
			const auto& face = srcMesh->mFaces[f];
			DEV_ASSERT(face.mNumIndices == 3);
			dstMesh.triangles[f] = *reinterpret_cast<engine::Mesh::Triangle*>(face.mIndices);
			indices.push_back(dstMesh.triangles[f]);
		}
	}

	model->init(vertices, indices);

}
}