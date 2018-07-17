#pragma once

#pragma once

#include "TestBase.h"
#include <math.h>
#include <random>

namespace SG
{

	class TestInput : public TestBase
	{
	private:

		SGScene2D * scene;
		SG::SGCommandBuffer2D* cmdBuffer;

		SG::SGCamera2D camera;
		SG::SGViewPort2D viewPort;
		SG::SGEntity2DHandle ent;
		SG::SGEntity2DHandle textEnt;

	public:
		TestInput(SG::SGEngine2D* toUse);
		virtual ~TestInput();

		void Update(float dt) override;
		void Render() override;

	};
}