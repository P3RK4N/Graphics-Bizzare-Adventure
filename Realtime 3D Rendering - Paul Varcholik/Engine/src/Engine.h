#pragma once

#include "Engine/Core/Application.h"
#include "Engine/Core/ApplicationException.h"
#include "Engine/Core/Core.h"
				 	 
#include "Engine/Core/RTTI.h"

#include "Engine/Components/ApplicationComponent.h"
#include "Engine/Components/DrawableApplicationComponent.h"
#include "Engine/Components/TextComponent.h"
#include "Engine/Components/TextPrinter.h"
#include "Engine/Components/FPSComponent.h"
#include "Engine/Components/Keyboard.h"
#include "Engine/Components/Mouse.h"
#include "Engine/Components/Camera.h"
#include "Engine/Components/FirstPersonCamera.h"
#include "Engine/Components/TriangleDemo.h"

using namespace DirectX;

#define DeleteObject(object)	if((object) != NULL) { delete object; object = NULL; }
#define DeleteObjects(objects)	if((objects) != NULL) { delete[] object; objects = NULL; }
#define ReleaseObject(object)	if((object) != NULL) { object->Release(); object = NULL; }