#pragma once
#include <memory>

#ifdef DEBUG
	#define ASSERT(x) if(!x) __debugbreak();
#else
	#define ASSERT(x)
#endif


#define EXPAND_MACRO(x) x
#define STRINGIFY_MACRO(x) #x
#define BIT(x) (1 << x)
#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1) 

#include "Engine/Core/ApplicationException.h"
#include "Engine/Core/RTTI.h"

namespace Engine
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args> 
	constexpr Scope<T> createScope(Args&& ... args) { return std::make_unique<T>(std::forward<Args>(args)...); }

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> createRef(Args&& ... args) { return std::make_shared<T>(std::forward<Args>(args)...); }

}

