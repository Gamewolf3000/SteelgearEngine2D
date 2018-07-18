#include <Windows.h>
#include <crtdbg.h>

#include "SGEngine2D.h"
#include "TestBasic.h"
#include "TestSpawner.h"
#include "TestScript.h"
#include "TestInput.h"
#include "TestGuiElement.h"
#include "TestGame.h"

int __stdcall WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(3114);

	SG::SGSettings2D settings;
	settings.graphicsSettings.threadedRendering = true;
	SG::SGEngine2D engine(settings);

	unsigned int nrOfFrames = 0;
	std::vector<SG::TestBase*> tests;
	int activeTest = 5;
	float dt = 0.0f;
	std::chrono::steady_clock::time_point start;
	std::chrono::steady_clock::time_point end;

	tests.push_back(new SG::TestBasic(&engine));
	tests.push_back(new SG::TestSpawner(&engine));
	tests.push_back(new SG::TestScript(&engine));
	tests.push_back(new SG::TestInput(&engine));
	tests.push_back(new SG::TestGuiElement(&engine));
	tests.push_back(new SG::TestGame(&engine));

	engine.Input()->AddSource(1000);
	engine.Input()->BindKey(1000, SG::KeyboardInput::KEY_ESC);

	for (int i = 0; i < tests.size(); i++)
	{
		engine.Input()->AddSource(1001 + i);
		engine.Input()->BindKey(1001 + i, (SG::KeyboardInput)(int(SG::KeyboardInput::KEY_0) + i));
	}

	while (engine.Input()->IsUp(1000))
	{
		start = std::chrono::steady_clock::now();
		auto windowRect = engine.Graphics()->GetWindow();
		engine.Input()->UpdateInput(windowRect);

		for (int i = 0; i < tests.size(); i++)
		{
			if (engine.Input()->IsDown(1001 + i))
				activeTest = i;
		}

		tests[activeTest]->Update(dt);
		tests[activeTest]->Render();

		nrOfFrames++;
		end = std::chrono::steady_clock::now();
		auto nrOfUnits = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		dt = nrOfUnits / 1000000000.0f;
	}

	for (auto& test : tests)
	{
		delete test;
	}

	return 0;
}