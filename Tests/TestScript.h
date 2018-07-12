#pragma once

#include "TestBase.h"
#include <math.h>
#include <random>

namespace SG
{

	class TestScript : public TestBase
	{
	private:

		SGScene2D * scene;
		SG::SGCommandBuffer2D* cmdBuffer;
		float timeTilSpawn = 0.0f;
		SG::SGEntity2DHandle textEnt;
		SG::SGScriptHandle script;

		SG::SGCamera2D camera;
		SG::SGViewPort2D viewPort;

	public:
		TestScript(SG::SGEngine2D* toUse);
		virtual ~TestScript();

		void Update(float dt) override;
		void Render() override;

	};
}