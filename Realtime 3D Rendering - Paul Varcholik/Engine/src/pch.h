#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <exception>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stdlib.h>

#include <d3d11_1.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <dinput.h>
#include <d3dx11effect.h>

#include "Engine/Core/Core.h"

#define DeleteObject(object)	if((object) != NULL) { delete object; object = NULL; }
#define DeleteObjects(objects)	if((objects) != NULL) { delete[] objects; objects = NULL; }
#define ReleaseObject(object)	if((object) != NULL) { object->Release(); object = NULL;}

namespace Engine
{
	typedef unsigned char byte;

	namespace Vector3Helper
	{
		static const DirectX::XMFLOAT3 zero = { 0.0f, 0.0f, 0.0f };
		static const DirectX::XMFLOAT3 forward = { 0.0f, 0.0f, 1.0f };
		static const DirectX::XMFLOAT3 up = { 0.0f, 1.0f, 0.0f };
		static const DirectX::XMFLOAT3 right = { -1.0f, 0.0f, 0.0f };
	}

	namespace Vector4Helper
	{
		/*DirectX::XMFLOAT4 getRandomColor()
		{
			return { (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, 1.0f };
		}*/
	}

	namespace Vector2Helper
	{
		static const DirectX::XMFLOAT2 zero = { 0.0f, 0.0f };
	}
}