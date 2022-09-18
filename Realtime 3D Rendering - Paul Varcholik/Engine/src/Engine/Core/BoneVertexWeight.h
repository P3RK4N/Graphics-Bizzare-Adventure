#pragma once

#include <vector>

#include <windows.h>

namespace Engine
{
	class BoneVertexWeights
	{
	public:
		struct VertexWeight
		{
			float weight = 0;
			UINT boneIndex = 0;
			VertexWeight(float weight, UINT index)
				: weight(weight), boneIndex(index) {}
			VertexWeight() = default;
		};
		
		const std::vector<VertexWeight>& getWeights() const { return m_Weights; }
		void addWeight(float weight, UINT boneIndex) { m_Weights.emplace_back(weight, boneIndex); }

		static const UINT s_MaxBoneWeightsPerVertex = 4;

	private:
		std::vector<VertexWeight> m_Weights{};
	};
}
