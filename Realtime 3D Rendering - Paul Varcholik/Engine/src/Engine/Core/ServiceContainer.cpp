#include "pch.h"
#include "ServiceContainer.h"

namespace Engine
{
	void ServiceContainer::addService(UINT typeID, void* service)
	{
		m_Services.insert(std::pair<UINT, void*>(typeID, service));
	}

	void ServiceContainer::removeService(UINT typeID)
	{
		m_Services.erase(typeID);
	}

	void* ServiceContainer::getService(UINT typeID) const
	{
		std::unordered_map<UINT, void*>::const_iterator it = m_Services.find(typeID);
		return it != m_Services.end() ? it->second : nullptr;
	}
}