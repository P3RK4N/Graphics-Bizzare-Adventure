#pragma once
#include "Engine/Core/RTTI.h"

#include "Engine/Shader/Effect.h"
#include "Engine/Shader/Variable.h"

namespace Engine
{
	class Model;
	class Mesh;
	
	class Material : public RTTI
	{
		RTTI_DECLARATIONS(Material, RTTI)

	public:
		Material() {}
		Material(const std::string& defaultTechniqueName)
			: m_DefaultTechniqueName(defaultTechniqueName) {}

		virtual ~Material();
		Variable* operator[](const std::string& variableName);
		Effect* getEffect() const { return m_Effect; }
		Technique* getCurrentTechnique() const { return m_CurrentTechnique; }
		void setCurrentTechnique(Technique* currentTechnique) { m_CurrentTechnique = currentTechnique; }
		const std::unordered_map<Pass*, ID3D11InputLayout*>& getInputLayouts() const { return m_InputLayouts; }

		virtual void initialize(Effect* effect);
		virtual void createVertexBuffer(ID3D11Device* device, const Model& model, std::vector<ID3D11Buffer*>& vertexBuffers) const;
		virtual void createVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const = 0;
		virtual UINT getVertexSize() const = 0;

	protected:
		Material(const Material&) = default;
		Material& operator=(const Material&) = default;

		virtual void createInputLayout(const std::string& techniqueName, const std::string& passName, D3D11_INPUT_ELEMENT_DESC* inputElementDescriptions, UINT inputElementDescriptionCount);

		Effect* m_Effect = nullptr;
		Technique* m_CurrentTechnique = nullptr;
		std::string m_DefaultTechniqueName{};
		std::unordered_map<Pass*, ID3D11InputLayout*> m_InputLayouts{};
	};

#define MATERIAL_VARIABLE_DECLARATION(var)		\
public:											\
	Variable& get ## var() const;				\
private:										\
	Variable* m_ ## var = nullptr;

#define MATERIAL_VARIABLE_DEFINITION(mat, var)	\
Variable& mat::get ## var() const				\
{												\
	return *m_ ## var;							\
}

#define MATERIAL_VARIABLE_INITIALIZATION(var) m_ ## var(NULL)
#define MATERIAL_VARIABLE_RETRIEVE(var) m_ ## var =  m_Effect->getVariablesByName().at(#var);
}