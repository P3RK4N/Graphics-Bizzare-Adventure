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

#include <d3d11_1.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include "Engine/Core.h"
#include "Engine/RTTI.h"


#define DeleteObject(object)	if((object) != NULL) { delete object; object = NULL; }
#define DeleteObjects(objects)	if((objects) != NULL) { delete[] object; objects = NULL; }
#define ReleaseObject(object)	if((object) != NULL) { object->Release(); object = NULL;}

namespace Engine
{
	typedef unsigned char byte;
}