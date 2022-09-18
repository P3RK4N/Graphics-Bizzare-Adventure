#pragma once
#include "Engine/Components/ApplicationComponent.h"
#include "Engine/Core/Model.h"
#include "Engine/Core/AnimationClip.h"
#include "Engine/Core/ApplicationTime.h"

namespace Engine
{
	class AnimationPlayer : public ApplicationComponent
	{
		RTTI_DECLARATIONS(AnimationPlayer, ApplicationComponent)

	public:
		AnimationPlayer(Application* app, Model* model, bool interpolationEnabled = true);

		const Model* getModel() const { return m_Model; }
		const AnimationClip* getCurrentClip() const { return m_CurrentClip; }
		float getCurrentTime() const { return m_CurrentTime; }
		UINT getCurrentKeyFrame() const { return m_CurrentKeyFrame; }
		const std::vector<XMFLOAT4X4>& getBoneTransforms() const { return m_FinalTransforms; }

		bool isInterpolationEnabled() const { return m_IsInterpolationEnabled; }
		bool isPlayingClip() const { return m_IsPlayingClip; }
		bool isClipLooped() const { return m_IsClipLooped; }

		void setInterpolationEnabled(bool interpolationEnabled) { m_IsInterpolationEnabled = interpolationEnabled; }
		void setCurrentKeyFrame(UINT keyFrame) { m_CurrentKeyFrame = keyFrame; getPoseAtKeyFrame(m_CurrentKeyFrame, *(m_Model->getRootNode())); }

		void startClip(AnimationClip& clip);
		void startClip();
		void pauseClip();
		void resumeClip();

		virtual void update(const ApplicationTime& applicationTime) override;

	private:
		AnimationPlayer() = default;
		AnimationPlayer(const AnimationPlayer&) = default;
		AnimationPlayer& operator=(const AnimationPlayer&) = default;

		void getBindPose(SceneNode& sceneNode);
		void getBindPose2(SceneNode& sceneNode);
		void getPose(float time, SceneNode& sceneNode);
		void getInterpolatedPose(float time, SceneNode& sceneNode);
		void getPoseAtKeyFrame(UINT keyFrame, SceneNode& sceneNode);

		Model* m_Model = nullptr;					
		AnimationClip* m_CurrentClip = nullptr;
		
		float m_CurrentTime = 0;
		UINT m_CurrentKeyFrame = 0;

		bool m_IsInterpolationEnabled = true;
		bool m_IsPlayingClip = false;
		bool m_IsClipLooped = true;			

		std::unordered_map<SceneNode*, XMFLOAT4X4> m_ToRootTransforms{};
		std::vector<XMFLOAT4X4> m_FinalTransforms{};
		XMFLOAT4X4 m_InverseRootTransform{};
	};
}