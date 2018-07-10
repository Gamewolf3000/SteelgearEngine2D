#pragma once

#include "../../SGG2D/SGRenderer2D.h"
#include "../../SG2DUtility/Headers/SGScene2D.h"
#include "SGTransformManager2D.h"

namespace SG
{
	class SGEngine2D
	{
	private:
		EventManager<BackEndEvent> eventManager;

		SGRenderer2D* graphics;

		SGTransformManager2D* transformManager;

	public:
		SGEngine2D(SGSettings2D settings);
		~SGEngine2D();

		inline SG::SGRenderer2D * Graphics()
		{
			return graphics;
		}

		inline SG::SGTransformManager2D * Transform()
		{
			return transformManager;
		}

		SGScene2D* CreateScene(unsigned int startNrOfEntities, unsigned int entitiesExpandAmount, SGGuid sceneIdentifier);
		void DestroyScene(SGScene2D* scene);

	};
}