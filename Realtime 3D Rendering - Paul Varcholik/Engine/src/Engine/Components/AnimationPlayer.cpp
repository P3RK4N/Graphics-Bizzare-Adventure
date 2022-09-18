#include "pch.h"
#include "AnimationPlayer.h"

namespace Engine
{
	RTTI_DEFINITIONS(AnimationPlayer)

	AnimationPlayer::AnimationPlayer(Application* app, Model* model, bool interpolationEnabled)
		: ApplicationComponent(app), m_Model(model), m_IsInterpolationEnabled(interpolationEnabled)
	{
		if(m_Model->getAnimationClipsByName().size())
		{
			m_CurrentClip = m_Model->getAnimationClipsByName().begin()->second;
			m_FinalTransforms.resize(m_Model->getBones().size(), {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1});
		}
			
	}

	void AnimationPlayer::startClip(AnimationClip& clip)
	{
		m_CurrentClip = &clip;
		m_CurrentTime = 0.0f;
		m_CurrentKeyFrame = 0;
		m_IsPlayingClip = true;

		XMVECTOR det = XMMatrixDeterminant(m_Model->getRootNode()->getTransformMatrix());

		XMMATRIX inverseRootTransform = XMMatrixInverse
		(
			&det,
			m_Model->getRootNode()->getTransformMatrix()
		);
		XMStoreFloat4x4(&m_InverseRootTransform, inverseRootTransform);
		getBindPose2(*(m_Model->getRootNode()));
	}

	void AnimationPlayer::startClip()
	{
		assert(m_CurrentClip);
		m_IsPlayingClip = true;
	}

	void AnimationPlayer::pauseClip()
	{
		m_IsPlayingClip = false;
	}

	void AnimationPlayer::resumeClip()
	{
		m_IsPlayingClip = true;
	}

	void AnimationPlayer::update(const ApplicationTime& applicationTime)
	{
		if(m_IsPlayingClip)
		{
			assert(m_CurrentClip != nullptr);
			SceneNode* rootNode = m_Model->getRootNode();

			m_CurrentTime += static_cast<float>(applicationTime.getElapsedApplicationTime()) * m_CurrentClip->getTicksPerSecond() * 1.0f;

			if(m_CurrentTime >= m_CurrentClip->getDuration())
			{
				if(m_IsClipLooped) m_CurrentTime = 150.66f;
				else
				{
					m_IsPlayingClip = false;
					return;
				}
			}


			if(m_IsInterpolationEnabled)
			{
				getInterpolatedPose(m_CurrentTime, *rootNode);
			}
			else
			{
				getPose(m_CurrentTime, *rootNode);
			}
		}
	}

	void AnimationPlayer::getBindPose2(SceneNode& sceneNode)
	{
		XMMATRIX toRootTransform = sceneNode.getTransformMatrix();
		SceneNode* parentNode = sceneNode.getParent();
		while (parentNode != nullptr)
		{
			toRootTransform = toRootTransform * parentNode->getTransformMatrix();
			parentNode = parentNode->getParent();
		}
		Bone* bone = sceneNode.as<Bone>();
		if (bone != nullptr)
		{
			XMStoreFloat4x4(&(m_FinalTransforms[bone->getIndex()]),
			bone->getOffsetTransformMatrix() * toRootTransform * XMLoadFloat4x4(&m_InverseRootTransform));
		}
		for (SceneNode* childNode : sceneNode.getChildren())
		{
			getBindPose2(*childNode);
		}
}

	void AnimationPlayer::getBindPose(SceneNode& sceneNode)
	{
		XMMATRIX toParentTransform = sceneNode.getTransformMatrix();
		XMMATRIX toRootTransform =
		(
			sceneNode.getParent() != nullptr ?
			toParentTransform * XMLoadFloat4x4(&m_ToRootTransforms.at(sceneNode.getParent())) :
			toParentTransform
		);
		XMStoreFloat4x4(&(m_ToRootTransforms[&sceneNode]), toRootTransform);

		Bone* bone = sceneNode.as<Bone>();
		if(bone != nullptr)
		{
			XMStoreFloat4x4
			(
				&(m_FinalTransforms[bone->getIndex()]),
				bone->getOffsetTransformMatrix() * toRootTransform * XMLoadFloat4x4(&m_InverseRootTransform)
			);

		}
		for(SceneNode* childNode : sceneNode.getChildren())
			getBindPose(*childNode);
	}

	void AnimationPlayer::getPose(float time, SceneNode& sceneNode)
	{
		XMFLOAT4X4 toParentTransform;
		Bone* bone = sceneNode.as<Bone>();
		if(bone != nullptr)
		{
			m_CurrentClip->getTransform(time, *bone, toParentTransform);
		}
		else
		{
			toParentTransform = sceneNode.getTransform();
		}

		XMMATRIX toRootTransform =
		(
			sceneNode.getParent() != nullptr ?
			XMLoadFloat4x4(&toParentTransform) * XMLoadFloat4x4(&(m_ToRootTransforms.at(sceneNode.getParent()))) :
			XMLoadFloat4x4(&toParentTransform)
		);

		XMStoreFloat4x4(&(m_ToRootTransforms[&sceneNode]), toRootTransform);

		if(bone != nullptr)
		{
			XMStoreFloat4x4
			(
				&(m_FinalTransforms[bone->getIndex()]), 
				bone->getOffsetTransformMatrix() * toRootTransform * XMLoadFloat4x4(&m_InverseRootTransform)
			);
		}

		for(SceneNode* childNode : sceneNode.getChildren())
			getPose(time, *childNode);
	}

	void AnimationPlayer::getInterpolatedPose(float time, SceneNode& sceneNode)
	{
		XMFLOAT4X4 toParentTransform;
		Bone* bone = sceneNode.as<Bone>();
		if(bone != nullptr)
		{
			m_CurrentClip->getInterpolatedTransform(time, *bone, toParentTransform);
		}
		else
		{
			toParentTransform = sceneNode.getTransform();
		}

		XMMATRIX toRootTransform =
		(
			sceneNode.getParent() != nullptr ?
			XMLoadFloat4x4(&toParentTransform) * XMLoadFloat4x4(&(m_ToRootTransforms.at(sceneNode.getParent()))) :
			XMLoadFloat4x4(&toParentTransform)
		);

		XMStoreFloat4x4(&(m_ToRootTransforms[&sceneNode]), toRootTransform);

		if(bone != nullptr)
		{
			XMStoreFloat4x4
			(
				&(m_FinalTransforms[bone->getIndex()]), 
				bone->getOffsetTransformMatrix() * toRootTransform * XMLoadFloat4x4(&m_InverseRootTransform)
			);
		}

		for(SceneNode* childNode : sceneNode.getChildren())
			getInterpolatedPose(time, *childNode);
	}

	void AnimationPlayer::getPoseAtKeyFrame(UINT keyFrame, SceneNode& sceneNode)
	{
		XMFLOAT4X4 toParentTransform;
		Bone* bone = sceneNode.as<Bone>();
		if(bone != nullptr)
		{
			m_CurrentClip->getTransformAtKeyFrame(keyFrame, *bone, toParentTransform);
		}
		else
		{
			toParentTransform = sceneNode.getTransform();
		}

		XMMATRIX toRootTransform =
		(
			sceneNode.getParent() != nullptr ?
			XMLoadFloat4x4(&toParentTransform) * XMLoadFloat4x4(&(m_ToRootTransforms.at(sceneNode.getParent()))) :
			XMLoadFloat4x4(&toParentTransform)
		);

		XMStoreFloat4x4(&(m_ToRootTransforms[&sceneNode]), toRootTransform);

		if(bone != nullptr)
		{
			XMStoreFloat4x4
			(
				&(m_FinalTransforms[bone->getIndex()]), 
				bone->getOffsetTransformMatrix() * toRootTransform * XMLoadFloat4x4(&m_InverseRootTransform)
			);
		}

		for(SceneNode* childNode : sceneNode.getChildren())
			getPoseAtKeyFrame(keyFrame, *childNode);
	}

}
