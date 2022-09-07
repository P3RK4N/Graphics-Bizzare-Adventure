#pragma once
#include "ApplicationComponent.h"

namespace Engine
{
	class Camera;

	class DrawableApplicationComponent : public ApplicationComponent
	{
		RTTI_DECLARATIONS(DrawableApplicationComponent, ApplicationComponent)

	public:
		DrawableApplicationComponent() 
			: ApplicationComponent() {}
		DrawableApplicationComponent(Application* app)
			: ApplicationComponent(app) {}
		DrawableApplicationComponent(Application* app, Camera* camera) 
			: ApplicationComponent(app), m_Camera(camera) {}

		virtual ~DrawableApplicationComponent() {}

		bool isVisible() const { return m_Visible; }
		void setVisible(bool visible) { m_Visible = visible; }

		Camera* getCamera() { return m_Camera; }
		void setCamera(Camera* camera) { m_Camera = camera; }

		virtual void draw(const ApplicationTime& applicationTime);

	protected:
		bool m_Visible = true;
		Camera* m_Camera = nullptr;

	private:
		DrawableApplicationComponent(const DrawableApplicationComponent& rhs) = default;
		DrawableApplicationComponent& operator=(const DrawableApplicationComponent& rhs) = default;
	};
}
