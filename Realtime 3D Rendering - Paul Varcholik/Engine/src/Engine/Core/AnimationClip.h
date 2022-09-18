#pragma once
#include "Bone.h"
#include "BoneAnimation.h"

#include <string>
#include <vector>
#include <unordered_map>

#include <windows.h>
#include <DirectXMath.h>
#include <assimp/anim.h>

using namespace DirectX;

namespace Engine
{
	class BoneAnimation;
	class Model;

	class AnimationClip
	{
		friend class Model;

	public:
		~AnimationClip() 
		{
			for(auto& boneAnimation : m_BoneAnimations)
				delete boneAnimation;
		}

		const std::string& getName() const { return m_Name; }
		float getDuration() const { return m_Duration; }
		float getTicksPerSecond() const { return m_TicksPerSecond; }
		const std::vector<BoneAnimation*>& getBoneAnimations() const { return m_BoneAnimations; }
		const std::unordered_map<Bone*, BoneAnimation*>& getBoneAnimationsByBone() const { return m_BoneAnimationsByBone; }
		const UINT getKeyFrameCount() const { return m_KeyFrameCount; }

		UINT getTransform(float time, Bone& bone, XMFLOAT4X4& transform) const;
		void getTransforms(float time, std::vector<XMFLOAT4X4>& boneTransforms) const;

		void getTransformAtKeyFrame(UINT keyFrame, Bone& bone, XMFLOAT4X4& transform) const;
		void getTransformsAtKeyFrame(UINT keyFrame, std::vector<XMFLOAT4X4>& boneTransforms) const;

		void getInterpolatedTransform(float time, Bone& bone, XMFLOAT4X4& transform) const;
		void getInterpolatedTransforms(float time, std::vector<XMFLOAT4X4>& boneTransforms) const;

	private:
		AnimationClip(Model& model, aiAnimation& animation);

		AnimationClip() = default;
		AnimationClip(const AnimationClip&) = default;
		AnimationClip& operator=(const AnimationClip&) = default;

		std::string m_Name{};
		float m_Duration = 0;
		float m_TicksPerSecond = 0;
		UINT m_KeyFrameCount = 0;
		std::vector<BoneAnimation*> m_BoneAnimations{};							
		std::unordered_map<Bone*, BoneAnimation*> m_BoneAnimationsByBone{};

	};
}