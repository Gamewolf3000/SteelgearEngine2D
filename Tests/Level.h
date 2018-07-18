#pragma once

#include "GameUnit.h"
#include <vector>

namespace SG
{
	class Level
	{
	private:
		SGEngine2D * engine;
		SGScene2D* scene;
		std::vector<SG::GameUnit*> enemies;
		std::vector<SG::SGEntity2DHandle> levelBlocks;

	public:
		Level(SG::SGEngine2D* engineToUse, SG::SGScene2D* sceneToUse);
		~Level();

		void Update(float dt, GameUnit* playerUnit);

	};
}