#include "pch.h"
#include "AnimationClip.h"

#include "Bone.h"
#include "Model.h"

namespace Engine
{
    AnimationClip::AnimationClip(Model& model, aiAnimation& animation)
        : m_Name(animation.mName.C_Str()), 
        m_Duration(static_cast<float>(animation.mDuration)),
        m_TicksPerSecond(animation.mTicksPerSecond)
    {
        assert(animation.mNumChannels > 0);

        if(m_TicksPerSecond <= 0.0f) m_TicksPerSecond = 1.0f;

        for(UINT i = 0; i < animation.mNumChannels; i++)
        {

            BoneAnimation* boneAnimation = new BoneAnimation(model, *(animation.mChannels[i]));
            m_BoneAnimations.push_back(boneAnimation);

            assert(m_BoneAnimationsByBone.find(boneAnimation->getBone()) == m_BoneAnimationsByBone.end());
            m_BoneAnimationsByBone[boneAnimation->getBone()] = boneAnimation;
        }

        for(BoneAnimation* boneAnimation : m_BoneAnimations)
        {
            if(boneAnimation->getKeyFrames().size() > m_KeyFrameCount)
            {
                m_KeyFrameCount = boneAnimation->getKeyFrames().size();
            }
        }
    }
    
    UINT AnimationClip::getTransform(float time, Bone& bone, XMFLOAT4X4& transform) const
    {
        auto foundBoneAnimation = m_BoneAnimationsByBone.find(&bone);
		if (foundBoneAnimation != m_BoneAnimationsByBone.end())
		{
			return foundBoneAnimation->second->getTransform(time, transform);
		}
		else
		{
			XMStoreFloat4x4(&transform, XMMatrixIdentity());
			return UINT_MAX;
		}
    }

    void AnimationClip::getTransforms(float time, std::vector<XMFLOAT4X4>& boneTransforms) const
    {
        for (BoneAnimation* boneAnimation : m_BoneAnimations)
		{
			boneAnimation->getTransform(time, boneTransforms[boneAnimation->getBone()->getIndex()]);
		}
    }

    void AnimationClip::getTransformAtKeyFrame(UINT keyFrame, Bone& bone, XMFLOAT4X4& transform) const
    {
        auto foundBoneAnimation = m_BoneAnimationsByBone.find(&bone);
		if (foundBoneAnimation != m_BoneAnimationsByBone.end())
		{
			foundBoneAnimation->second->getTransformAtKeyFrame(keyFrame, transform);
		}
		else
		{
			XMStoreFloat4x4(&transform, XMMatrixIdentity());
		}
    }

    void AnimationClip::getTransformsAtKeyFrame(UINT keyFrame, std::vector<XMFLOAT4X4>& boneTransforms) const
    {
        for (BoneAnimation* boneAnimation : m_BoneAnimations)
		{
			boneAnimation->getTransformAtKeyFrame(keyFrame, boneTransforms[boneAnimation->getBone()->getIndex()]);
		}
    }

    void AnimationClip::getInterpolatedTransform(float time, Bone& bone, XMFLOAT4X4& transform) const
    {
        auto foundBoneAnimation = m_BoneAnimationsByBone.find(&bone);
		if (foundBoneAnimation != m_BoneAnimationsByBone.end())
		{
			foundBoneAnimation->second->getInterpolatedTransform(time, transform);
		}
		else
		{
			XMStoreFloat4x4(&transform, XMMatrixIdentity());
		}
    }

    void AnimationClip::getInterpolatedTransforms(float time, std::vector<XMFLOAT4X4>& boneTransforms) const
    {
        for (BoneAnimation* boneAnimation : m_BoneAnimations)
		{
			boneAnimation->getInterpolatedTransform(time, boneTransforms[boneAnimation->getBone()->getIndex()]);
		}
    }

}
