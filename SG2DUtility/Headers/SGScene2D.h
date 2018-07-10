#pragma once

#include "SGEntityManager2D.h"
#include "SGBackEndData.h"
#include "DebugMessageHandler.h"
#include "EventManager.h"

namespace SG
{

	class SGScene2D
	{
	private:
		friend class SGEngine2D;
		DebugMessageHandler* debugHandler;

		EventManager<BackEndEvent>* eventHandler;
		SGEntityManager2D entityManager;

		SGScene2D(unsigned int startNrOfEntities, unsigned int entitiesExpandAmount, EventManager<BackEndEvent>* eventsBackEnd, SGGuid sceneIdentifier);

	public:
		SGScene2D() = delete;
		SGScene2D(const SGScene2D& other) = delete;
		~SGScene2D();

		SGEntityManager2D& GetEntityManager();

		SGSceneEntities GetSceneEntities();
	};
}