#include "RenderingGame.h"
#include "PostProcessing.h"
#include "ShadowMapping.h"
#include "AnimationGame.h"
#include "Geometry_Tesselation_Game.h"
#include "ComputeGame.h"

#include <memory>

#define _DEBUG

#ifdef DEBUG || _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif // DEBUG || _DEBUG

using namespace Engine;

INT WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int showCommand)
{
#ifdef DEBUG || _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	Scope<ShadowMapping> app = createScope<ShadowMapping>(instance, L"RenderingGame", L"Real-Time 3D Rendering", showCommand);

	try
	{
		app->run();
	}
	catch (ApplicationException e)
	{
		MessageBox(app->getWindowHandle(), (LPCWSTR)e.what(), app->getWindowTitle().c_str(), MB_ABORTRETRYIGNORE);
	}

	return 0;
}