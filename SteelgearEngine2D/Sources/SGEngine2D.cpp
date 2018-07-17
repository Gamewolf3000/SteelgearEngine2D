#include "../../SteelgearEngine2D/Headers/SGEngine2D.h"
#include "DebugMessageHandler.h"
#include "../../SGG2D/SGRendererSFML.h"
#include "../SGInput/SGInputHandlerSFML.h"

SG::SGEngine2D::SGEngine2D(SGSettings2D settings)
{
	graphics = new SGRendererSFML(settings.graphicsSettings, &eventManager);
	transformManager = new SGTransformManager2D(10, 10, &eventManager);
	scriptManager = new SGScriptManagerAS();
	inputHandler = new SGInputHandlerSFML();

}

SG::SGEngine2D::~SGEngine2D()
{
	graphics->WaitForThread();
	delete graphics;
	delete scriptManager;
	delete transformManager;
	delete inputHandler;
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

SG::SGGuiElement SG::SGEngine2D::CreateGuiElement(SGScene2D* sceneToAddTo, SGGuiSettings& elementSettings)
{
	auto ent = sceneToAddTo->GetEntityManager().CreateEntity();
	transformManager->SetPos(ent, elementSettings.xPos, elementSettings.yPos);
	graphics->ShapeManager()->CreateRectangleShape(ent, elementSettings.xSize, elementSettings.ySize);
	
	if(!(elementSettings.textureNotSelected == SGGuid("")))
		graphics->TextureManager()->SetTexture(ent, elementSettings.textureNotSelected);

	if(!(elementSettings.text.font == SGGuid("")))
		graphics->TextManager()->CreateText(ent, elementSettings.text);

	if (!(elementSettings.pipeline == SGGuid("")))
		graphics->ShaderManager()->SetRenderPipelineOfEntity(ent, elementSettings.pipeline);

	SGGuiElement temp(ent, graphics, scriptManager, inputHandler);

	if(elementSettings.scriptPath.length() > 0)
		temp.script = scriptManager->LoadScript(elementSettings.scriptPath.c_str());

	temp.selectedFunctionName = elementSettings.selectedFunctionName;
	temp.updateFunctionName = elementSettings.updateFunctionName;
	temp.textureNotSelected = elementSettings.textureNotSelected;
	temp.textureSelected = elementSettings.textureSelected;
	temp.checkForInput = elementSettings.checkForInput;

	return temp;
}