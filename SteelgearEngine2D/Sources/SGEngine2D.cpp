#include "../../SteelgearEngine2D/Headers/SGEngine2D.h"
#include "DebugMessageHandler.h"
#include "../../SGG2D/SGRendererSFML.h"

SG::SGEngine2D::SGEngine2D(SGSettings2D settings)
{
	graphics = new SGRendererSFML(settings.graphicsSettings, &eventManager);
	transformManager = new SGTransformManager2D(10, 10, &eventManager);
}

SG::SGEngine2D::~SGEngine2D()
{
	graphics->WaitForThread();
	delete graphics;
	delete transformManager;
	DebugMessageHandler::DestroyInstance();
}

SG::SGScene2D* SG::SGEngine2D::CreateScene(unsigned int startNrOfEntities, unsigned int entitiesExpandAmount, SGGuid sceneIdentifier)
{
	return new SGScene2D(startNrOfEntities, entitiesExpandAmount, &eventManager, sceneIdentifier);
}

void SG::SGEngine2D::DestroyScene(SGScene2D * scene)
{
	delete scene;
}
