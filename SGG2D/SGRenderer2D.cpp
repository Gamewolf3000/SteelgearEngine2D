#include "SGRenderer2D.h"

const SG::SGVector<SG::SGEntity2D>& SG::SGRenderer2D::GetEntitiesFromScene(const SGSceneEntities& sceneEntities)
{
	return sceneEntities.entities;
}

SG::SGGuid SG::SGRenderer2D::GetSceneID(const SGSceneEntities & sceneEntities)
{
	return sceneEntities.handlerID;
}

std::vector<unsigned int> SG::SGRenderer2D::GetSceneOrdering(const SGSceneEntities & sceneEntities)
{
	return sceneEntities.sorting;
}

void SG::SGRenderer2D::CleanUpManagers()
{
	shapeManager->CleanUp();
	textureManager->CleanUp();
	shaderManager->CleanUp();
}

void SG::SGRenderer2D::UpdateManagers()
{
	shapeManager->UpdateData();
	textureManager->UpdateData();
	shaderManager->UpdateData();
}

void SG::SGRenderer2D::ExecuteStoredCommands(int toUse)
{
	for (auto& job : commandBuffers[toUse].jobs)
	{
		job.function();
	}
}

SG::SGRenderer2D::SGRenderer2D(const SGGraphicSettings2D & gSettings, EventManager<BackEndEvent>* eventsBackEnd) : settings(gSettings)
{
	if (gSettings.threadedRendering)
	{
		threadedRendering = true;
		threadIsActive = true;
	}

	debugHandler = DebugMessageHandler::GetInstance();
}

SG::SGRenderer2D::~SGRenderer2D()
{
	delete shapeManager;
	delete textureManager;
	delete shaderManager;
	delete textManager;
}

SG::GraphicsJob SG::SGRenderer2D::CreateClearRenderTargetJob(SGGuid identifier, const SGColour & clearColour)
{
	GraphicsJob temp;
	temp.identifier = identifier;
	temp.function = { std::bind(&SGRenderer2D::ClearRenderTarget, this, clearColour) };

	return temp;
}

SG::GraphicsJob SG::SGRenderer2D::CreateRenderSceneJob(SGGuid identifier, SGScene2D & scene, const SGCamera2D & camera, const SGViewPort2D & viewPort)
{
	GraphicsJob temp;
	temp.identifier = identifier;
	temp.function = { std::bind(&SGRenderer2D::RenderScene, this, scene.GetSceneEntities(), camera, viewPort) };

	return temp;
}

SG::GraphicsJob SG::SGRenderer2D::CreateDisplayJob(SGGuid identifier)
{
	GraphicsJob temp;
	temp.identifier = identifier;
	temp.function = { std::bind(&SGRenderer2D::Display, this) };

	return temp;
}

void SG::SGRenderer2D::ExecuteCommandBuffer(const SGCommandBuffer2D & commandBuffer)
{
	HandleWindowEvents();

	if (settings.threadedRendering)
	{
		threadMutex.lock();
		hasUpdatedBuffer = true;
		//debugHandler->StoreMessage("Updating constant buffer");
		commandBuffers[inactiveCommandBuffer] = commandBuffer;
		UpdateManagers();

		threadMutex.unlock();
	}
	else
	{	
		UpdateManagers();
		CleanUpManagers();

		for (auto& job : commandBuffer.jobs)
		{
			job.function();
		}
	}
}
