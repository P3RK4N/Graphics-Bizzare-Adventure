#pragma once
#include "Engine/Core/Model.h"

#include <d3d11.h>
#include <DirectXMath.h>
#include <windows.h>

using namespace DirectX;

struct aiMesh;

namespace Engine
{
	class Material;
	class ModelMaterial;

	class Mesh
	{
		friend class Model;

	public:
		Mesh(Model& model, ModelMaterial* material)
			: m_Model(model), m_Material(material) {}
		~Mesh();

		Model& getModel() { return m_Model; }
		ModelMaterial* getMaterial() { return m_Material; }
		const std::string& getName() const { return m_Name; }

		const std::vector<XMFLOAT3>& getVertices() const { return m_Vertices; }
		const std::vector<UINT>& getIndices() const { return m_Indices; }
		const std::vector<XMFLOAT3>& getNormals() const { return m_Normals; }
		const std::vector<XMFLOAT3>& getTangents() const { return m_Tangents; }
		const std::vector<XMFLOAT3>& getBiNormals() const { return m_BiNormals; }
		const std::vector<std::vector<XMFLOAT3>*>& getTextureCoordinates() const { return m_TextureCoordinates; }
		const std::vector<std::vector<XMFLOAT4>*>& getVertexColors() const { return m_VertexColors; }

		UINT getFaceCount() const { return m_FaceCount; }

		void createIndexBuffer(ID3D11Buffer** indexBuffer);

	private:
		Mesh(Model& model, aiMesh& mesh);
		Mesh(const Mesh&) = default;
		Mesh& operator=(const Mesh&) = default;

		Model& m_Model;
		ModelMaterial* m_Material = nullptr;
		std::string m_Name{};
		std::vector<XMFLOAT3> m_Vertices{};
		std::vector<UINT> m_Indices{};									
		std::vector<XMFLOAT3> m_Normals{};
		std::vector<XMFLOAT3> m_Tangents{};
		std::vector<XMFLOAT3> m_BiNormals{};
		std::vector<std::vector<XMFLOAT3>*> m_TextureCoordinates{};
		std::vector<std::vector<XMFLOAT4>*> m_VertexColors{};
		UINT m_FaceCount = 0;
	};
}
