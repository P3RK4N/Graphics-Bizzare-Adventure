#include "pch.h"
#include "BoneAnimation.h"
#include "Model.h"

namespace Engine
{
    BoneAnimation::BoneAnimation(Model& model, aiNodeAnim& nodeAnim)
        : m_Model(&model)
    {
        UINT boneIndex = m_Model->m_BoneIndexMapping.at(nodeAnim.mNodeName.C_Str());
        m_Bone = model.m_Bones.at(boneIndex);

        assert(nodeAnim.mNumPositionKeys == nodeAnim.mNumRotationKeys);
        assert(nodeAnim.mNumPositionKeys == nodeAnim.mNumScalingKeys);

        for(UINT i = 0; i < nodeAnim.mNumPositionKeys; i++)
        {
            aiVectorKey positionKey = nodeAnim.mPositionKeys[i];
            aiQuatKey rotationKey = nodeAnim.mRotationKeys[i];
            aiVectorKey scaleKey = nodeAnim.mScalingKeys[i];

            assert(positionKey.mTime == rotationKey.mTime);
            assert(positionKey.mTime == scaleKey.mTime);
            
            KeyFrame* keyFrame = new KeyFrame
            (
                static_cast<float>(positionKey.mTime),
                XMFLOAT3(positionKey.mValue.x, positionKey.mValue.y, positionKey.mValue.z),
                XMFLOAT4(rotationKey.mValue.x, rotationKey.mValue.y, rotationKey.mValue.z, rotationKey.mValue.w),
                XMFLOAT3(scaleKey.mValue.x, scaleKey.mValue.y, scaleKey.mValue.z)
            );

            m_KeyFrames.push_back(keyFrame);
        }

    }

    UINT BoneAnimation::getTransform(float time, XMFLOAT4X4& transform) const
    {
        UINT keyFrameIndex = findKeyFrameIndex(time);
        KeyFrame* keyFrame = m_KeyFrames[keyFrameIndex];
        XMStoreFloat4x4(&transform, keyFrame->getTransform());
        return keyFrameIndex;
    }

    void BoneAnimation::getTransformAtKeyFrame(UINT keyFrameIndex, XMFLOAT4X4& transform) const
    {
        if (keyFrameIndex >= m_KeyFrames.size())
        {
            keyFrameIndex = m_KeyFrames.size() - 1;
        }

        KeyFrame* keyFrame = m_KeyFrames[keyFrameIndex];
        XMStoreFloat4x4(&transform, keyFrame->getTransform());
    }

    void BoneAnimation::getInterpolatedTransform(float time, XMFLOAT4X4& transform) const
    {
        KeyFrame* firstKeyFrame = m_KeyFrames.front();
        KeyFrame* lastKeyFrame = m_KeyFrames.back();

        if(time <= firstKeyFrame->getTime())
            XMStoreFloat4x4(&transform, firstKeyFrame->getTransform());
        else if(time >= lastKeyFrame->getTime())
            XMStoreFloat4x4(&transform, lastKeyFrame->getTransform());
        else
        {
            UINT keyframeIndex = findKeyFrameIndex(time);
            KeyFrame* keyFrameOne = m_KeyFrames[keyframeIndex];
            KeyFrame* keyFrameTwo = m_KeyFrames[keyframeIndex + 1];
            XMVECTOR translationOne = keyFrameOne->getTranslationVector();
            XMVECTOR rotationOne = keyFrameOne->getRotationVector();
            XMVECTOR scaleOne = keyFrameOne->getScaleVector();
            XMVECTOR translationTwo = keyFrameTwo->getTranslationVector();
            XMVECTOR rotationTwo = keyFrameTwo->getRotationVector();
            XMVECTOR scaleTwo = keyFrameTwo->getScaleVector();

            float lerpValue = ((time - keyFrameOne->getTime()) / (keyFrameTwo->getTime() - keyFrameOne->getTime()));

            XMVECTOR translation = XMVectorLerp(translationOne, translationTwo, lerpValue);
            XMVECTOR rotation = XMQuaternionSlerp(rotationOne, rotationTwo, lerpValue);
            XMVECTOR scale = XMVectorLerp(scaleOne, scaleTwo, lerpValue);

            XMFLOAT4 origin{ 0.0f, 0.0f, 0.0f, 0.0f };
            XMVECTOR rotationOrigin = XMLoadFloat4(&origin);
			XMStoreFloat4x4(&transform, XMMatrixAffineTransformation(scale, rotationOrigin, rotation, translation));
        }
    }

    UINT BoneAnimation::findKeyFrameIndex(float time) const
    {
        KeyFrame* firstKeyFrame = m_KeyFrames.front();
        if (time <= firstKeyFrame->getTime())
        {
            return 0;
        }
        KeyFrame* lastKeyFrame = m_KeyFrames.back();
        if (time >= lastKeyFrame->getTime())
        {
            return m_KeyFrames.size() - 1;
        }

        UINT keyframeIndex = 1;
        for (; keyframeIndex < m_KeyFrames.size() - 1 && time >= m_KeyFrames[keyframeIndex]->getTime(); keyframeIndex++);
        return keyframeIndex - 1;
    }
    
}
