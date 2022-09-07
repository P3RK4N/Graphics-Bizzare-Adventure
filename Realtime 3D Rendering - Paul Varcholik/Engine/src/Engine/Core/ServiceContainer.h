#pragma once
#include <unordered_map>

#include <windows.h>

namespace Engine
{
	class ServiceContainer
	{
	public:
		ServiceContainer() {}

		void addService(UINT typeID, void* service);
		void removeService(UINT typeID);

		void* getService(UINT typeID) const;

	private:
		ServiceContainer(const ServiceContainer&) = default;
		ServiceContainer& operator=(const ServiceContainer&) = default;

		std::unordered_map<UINT, void*> m_Services{};
	};
}