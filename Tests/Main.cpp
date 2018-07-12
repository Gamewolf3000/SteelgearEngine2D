#include <Windows.h>
#include <crtdbg.h>

#include "SGEngine2D.h"
#include "TestBasic.h"
#include "TestSpawner.h"
#include "TestScript.h"

int __stdcall WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(3114);

	SG::SGSettings2D settings;
	settings.graphicsSettings.threadedRendering = true;
	SG::SGEngine2D engine(settings);

	unsigned int nrOfFrames = 0;
	std::vector<SG::TestBase*> tests;
	int activeTest = 2;

	tests.push_back(new SG::TestBasic(&engine));
	tests.push_back(new SG::TestSpawner(&engine));
	tests.push_back(new SG::TestScript(&engine));

	while (!(GetKeyState(VK_ESCAPE) & 0x8000))
	{
		for (int i = 0; i < tests.size(); i++)
		{
			if ((GetKeyState('0' + i) & 0x8000))
				activeTest = i;
		}

		tests[activeTest]->Update(0.0001f);
		tests[activeTest]->Render();

		nrOfFrames++;
	}

	for (auto& test : tests)
	{
		delete test;
	}

	return 0;
}