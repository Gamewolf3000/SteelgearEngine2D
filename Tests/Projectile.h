#pragma once

#include "../SG2DUtility/Headers/SGEntity2D.h"
#include "../SG2DUtility/Headers/SGGuid.h"
#include "../SGScripts/SGScriptManagerAS.h"
#include "../SteelgearEngine2D/Headers/SGEngine2D.h"

namespace SG
{
	class Projectile
	{
	private:
		SGEntity2DHandle ent;
		SGScriptHandle script;

		int damage;

	public:
		Projectile(SGEntity2DHandle entToUse, SGScriptHandle scriptToUse)
		{
			ent = entToUse;
			script = scriptToUse;
		}

		virtual ~Projectile();

		void Update(float dt, SGEngine2D* engine)
		{
			engine->Script()->ExecuteScript(script, "void Update(float dt, Projectile &in myProjectile)", dt, this);
		}

		SG::SGEntity2DHandle& GetEntity()
		{
			return ent;
		}

		int GetDamage()
		{
			return damage;
		}

	};
}