#pragma once

#include "Engine/Application.h"
#include "Engine/ApplicationException.h"
#include "Engine/Core.h"

#include "Engine/RTTI.h"

#include "Engine/ApplicationComponent.h"
#include "Engine/DrawableApplicationComponent.h"
#include "Engine/TextComponent.h"
#include "Engine/TextPrinter.h"
#include "Engine/FPSComponent.h"
#include "Engine/Keyboard.h"
#include "Engine/Mouse.h"

#define DeleteObject(object)	if((object) != NULL) { delete object; object = NULL; }
#define DeleteObjects(objects)	if((objects) != NULL) { delete[] object; objects = NULL; }
#define ReleaseObject(object)	if((object) != NULL) { object->Release(); object = NULL; }