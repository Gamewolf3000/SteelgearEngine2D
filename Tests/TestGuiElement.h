#pragma once

#pragma once

#include "TestBase.h"
#include <math.h>
#include <random>

namespace SG
{

	class TestGuiElement : public TestBase
	{
	private:

		SGScene2D * scene;
		SG::SGCommandBuffer2D* cmdBuffer;
		float timeTilSpawn = 0.0f;
		SG::SGGuiElement button;

		SG::SGCamera2D camera;
		SG::SGViewPort2D viewPort;

	public:
		TestGuiElement(SG::SGEngine2D* toUse);
		virtual ~TestGuiElement();

		void Update(float dt) override;
		void Render() override;

	};
}