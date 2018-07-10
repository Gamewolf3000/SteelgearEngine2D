#pragma once

#include "TestBase.h"

namespace SG
{

	class TestBasic : public TestBase
	{
	private:
		SGScene2D * scene;
		SG::SGCommandBuffer2D* cmdBuffer;
		std::vector<SGEntity2DHandle> entHandles;

		SG::SGCamera2D camera;
		SG::SGViewPort2D viewPort;

	public:
		TestBasic(SG::SGEngine2D* toUse);
		virtual ~TestBasic();

		void Update(float dt) override;
		void Render() override;

	};
}