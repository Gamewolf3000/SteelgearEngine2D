#pragma once

#include "SGEngine2D.h"

namespace SG
{

	class TestBase
	{
	protected:
		SG::SGEngine2D* engine;

	public:
		TestBase(SG::SGEngine2D* toUse) { engine = toUse; };
		virtual ~TestBase() {};

		virtual void Update(float dt) = 0;
		virtual void Render() = 0;

	};
}