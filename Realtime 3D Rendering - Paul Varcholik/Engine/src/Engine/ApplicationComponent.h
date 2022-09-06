#pragma once
#include "RTTI.h"

namespace Engine
{
	class Application;
	class ApplicationTime;

	class ApplicationComponent : public RTTI
	{
		RTTI_DECLARATIONS(ApplicationComponent, RTTI)

	public:
		ApplicationComponent()
			: m_Application(nullptr), m_Enabled(true) {}
		ApplicationComponent(Application* app)
			: m_Application(app), m_Enabled(true) {}

		virtual ~ApplicationComponent() {}

		Application* getApplication() { return m_Application; }
		void setApplication(Application& app) { m_Application = &app; }
		bool isEnabled() const { return m_Enabled; }
		void setEnabled(bool enabled) { m_Enabled = enabled; }

		virtual void initialize();
		virtual void update(const ApplicationTime& applicationTime);

	protected:
		Application* m_Application;
		bool m_Enabled;

	private:
		ApplicationComponent(const ApplicationComponent& rhs) = default;
		ApplicationComponent& operator=(const ApplicationComponent& rhs) = default;
	};
}