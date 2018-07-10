#include "../Headers/SGScene2D.h"

SG::SGScene2D::SGScene2D(unsigned int startNrOfEntities, unsigned int entitiesExpandAmount, EventManager<BackEndEvent>* eventsBackEnd, SGGuid sceneIdentifier)
	: entityManager(startNrOfEntities, entitiesExpandAmount, eventsBackEnd, sceneIdentifier), eventHandler(eventsBackEnd)
{
	debugHandler = DebugMessageHandler::GetInstance();
}

SG::SGScene2D::~SGScene2D()
{
}

SG::SGEntityManager2D & SG::SGScene2D::GetEntityManager()
{
	return entityManager;
}

SG::SGSceneEntities SG::SGScene2D::GetSceneEntities()
{
	//debugHandler->StoreMessage("Updating commandBuffer");

	//for (int i = 0; i < entityManager.objects.Size(); i++)
	//{
	//	debugHandler->StoreMessage(std::string(std::string("Entity ") + std::to_string(entityManager.objects[i].id) + std::string(" with shape ") + std::to_string(entityManager.objects[i].components[int(ComponentType::COMPONENT_SHAPE)])).c_str());
	//}

	entityManager.SortEntities();
	return SGSceneEntities(entityManager.objects, entityManager.sceneGuid, entityManager.sortedEntities);
}
