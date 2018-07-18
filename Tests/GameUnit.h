#pragma once

#include "../SG2DUtility/Headers/SGEntity2D.h"
#include "../SG2DUtility/Headers/SGGuid.h"
#include "../SGScripts/SGScriptManagerAS.h"
#include "../SteelgearEngine2D/Headers/SGEngine2D.h"

namespace SG
{
	class GameUnit
	{
	private:
		SGEngine2D* engine;
		SGEntity2DHandle ent;
		SGScriptHandle script;

		SGRect previousRect;
		int health;

	public:
		GameUnit()
		{

		}

		GameUnit(SGEngine2D* engineToUse, SGEntity2DHandle entToUse, SGScriptHandle scriptToUse)
		{
			engine = engineToUse;
			ent = entToUse;
			script = scriptToUse;
		}

		~GameUnit()
		{

		}

		void Update(float dt)
		{
			previousRect = engine->Graphics()->ShapeManager()->GetShapeRect(ent);
			engine->Script()->ExecuteScript(script, "void Update(float dt, GameUnit &in myUnit)", dt, this);
		}

		SG::SGEntity2DHandle& GetEntity()
		{
			return ent;
		}

		int GetHealth()
		{
			return health;
		}

		SGPoint GetPosition()
		{
			return engine->Transform()->GetPos(ent);
		}

		void ReduceHealth(int value)
		{
			health -= value;
		}

		void ResetPos(bool x, bool y)
		{
			auto currentRect = engine->Graphics()->ShapeManager()->GetShapeRect(ent);
			float xToSet;
			float yToSet;

			if (x)
			{
				xToSet = previousRect.leftMost + previousRect.width / 2.0f;
			}
			else
			{
				xToSet = currentRect.leftMost + currentRect.width / 2.0f;
			}

			if (y)
			{
				yToSet = previousRect.topMost + previousRect.height / 2.0f;
			}
			else
			{
				yToSet = currentRect.topMost + currentRect.height / 2.0f;
			}

			engine->Transform()->SetPos(ent, xToSet, yToSet);

		}

		void Move(float xToMove, float yToMove)
		{
			engine->Transform()->Move(ent, xToMove, yToMove);
		}

		SGRect GetPreviousRect()
		{
			return previousRect;
		}

	};
}