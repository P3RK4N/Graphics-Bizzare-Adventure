#pragma once

#include "Bone.h"
#include "Keyframe.h"

#include <assimp/anim.h>

namespace Engine
{
	class Model;

	class BoneAnimation
	{
		friend class AnimationClip;

	public:
		~BoneAnimation() {}

		Bone* getBone() { return m_Bone; }
		const std::vector<KeyFrame*> getKeyFrames() const { return m_KeyFrames; }

		UINT getTransform(float time, XMFLOAT4X4& transform) const;
		void getTransformAtKeyFrame(UINT keyFrameIndex, XMFLOAT4X4& transform) const;
		void getInterpolatedTransform(float time, XMFLOAT4X4& transform) const;

	private:
		BoneAnimation(Model& model, aiNodeAnim& nodeAnim);

		BoneAnimation() = default;
		BoneAnimation(const BoneAnimation&) = default;
		BoneAnimation& operator=(const BoneAnimation&) = default;

		UINT findKeyFrameIndex(float time) const;

		Model* m_Model = nullptr;
		Bone* m_Bone = nullptr;
		std::vector<KeyFrame*> m_KeyFrames{};
	};
}