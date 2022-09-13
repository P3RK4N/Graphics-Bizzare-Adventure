#pragma once

#include "Engine/Core/Application.h"
#include "Engine/Core/ApplicationException.h"
#include "Engine/Core/Core.h"
#include "Engine/Core/FullScreenRenderTarget.h"
#include "Engine/Core/RenderTarget.h"
#include "Engine/Core/DepthMap.h"
				 	 
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
#include "Engine/Components/CubeDemo.h"
#include "Engine/Components/ModelDemo.h"
#include "Engine/Components/TexturedModelDemo.h"
#include "Engine/Components/MaterialDemo.h"
#include "Engine/Components/Skybox.h"
#include "Engine/Components/DiffuseLightingDemo.h"
#include "Engine/Components/PointLightDemo.h"
#include "Engine/Components/FullScreenQuad.h"
#include "Engine/Components/ProjectionDemo.h"

#include "Engine/Materials/PostProcessMaterial.h"

using namespace DirectX;

#define DeleteObject(object)	if((object) != NULL) { delete object; object = NULL; }
#define DeleteObjects(objects)	if((objects) != NULL) { delete[] objects; objects = NULL; }
#define ReleaseObject(object)	if((object) != NULL) { object->Release(); object = NULL; }