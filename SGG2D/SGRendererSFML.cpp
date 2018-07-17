#include "SGRendererSFML.h"
#include "SGShapeManagerSFML.h"
#include "SGTextureManager2DSFML.h"
#include "SGShaderHandler2DSFML.h"
#include "SGTextManager2DSFML.h"

#include "SFML\Graphics.hpp"

SG::SGRendererSFML::SGRendererSFML(SGGraphicSettings2D gSettings, EventManager<BackEndEvent>* eventsBackEnd) : SGRenderer2D(gSettings, eventsBackEnd)
{
	this->shapeManager = new SGShapeManagerSFML(10, 10, eventsBackEnd);
	this->textureManager = new SGTextureManager2DSFML(10, 10, eventsBackEnd);
	this->shaderManager = new SGShaderHandler2DSFML(10, 10, eventsBackEnd);
	this->textManager = new SGTextManager2DSFML(10, 10, eventsBackEnd);
	this->window = new sf::RenderWindow(sf::VideoMode(gSettings.screenWidth, gSettings.screenHeight), gSettings.windowTitle);

	if (settings.threadedRendering)
	{
		window->setActive(false);
		std::thread temp(std::bind(&SGRendererSFML::RenderThread, this));
		temp.detach();
	}
}

SG::SGRendererSFML::~SGRendererSFML()
{
	delete window;
}

void SG::SGRendererSFML::RenderThread()
{
	window->setActive(true);
	while (threadedRendering)
	{
		threadMutex.lock();
		//debugHandler->StoreMessage("Cleaning up and switching commandbuffer");

		if (hasUpdatedBuffer)
		{
			CleanUpManagers();
			int toUse = inactiveCommandBuffer;

			if (inactiveCommandBuffer == 0)
				inactiveCommandBuffer = 1;
			else
				inactiveCommandBuffer = 0;

			hasUpdatedBuffer = false;
			threadMutex.unlock();

			ExecuteStoredCommands(toUse);
		}
		else
		{
			threadMutex.unlock(); // If the cpu thread has not fed another commandbuffer yet then we just unlock the mutex and try again
		}
	}

	threadIsActive = false;
}

void SG::SGRendererSFML::HandleWindowEvents()
{
	sf::Event windowEvent;
	while (window->pollEvent(windowEvent))
	{
		if (windowEvent.type == sf::Event::Closed)
		{
			//window.close();
		}
	}
}

void SG::SGRendererSFML::ClearRenderTarget(const SGColour & clearColour)
{
	window->clear(sf::Color(clearColour.red, clearColour.green, clearColour.blue));
}

void SG::SGRendererSFML::RenderScene(const SGSceneEntities& sceneEntities, const SGCamera2D & camera, const SGViewPort2D & viewPort)
{
	sf::View view(sf::Vector2f(camera.xPos, camera.yPos), sf::Vector2f(camera.width, camera.height));
	view.setViewport(sf::FloatRect(viewPort.left, viewPort.top, viewPort.width, viewPort.height));
	window->setView(view);

	auto ents = GetEntitiesFromScene(sceneEntities);
	SGGuid sceneId = GetSceneID(sceneEntities);
	auto order = GetSceneOrdering(sceneEntities);
	int currentPipeline = -1;

	//debugHandler->StoreMessage(std::string(std::string("Starting to render scene with the following entities - in total ") + std::to_string(ents.Size())).c_str());
	//
	//for (int i = 0; i < ents.Size(); i++)
	//{
	//	debugHandler->StoreMessage(std::string(std::string("Entity ") + std::to_string(ents[order[i]].id) + std::string(" with shape ") + std::to_string(ents[order[i]].components[int(ComponentType::COMPONENT_SHAPE)])).c_str());
	//}

	for (int i = 0; i < order.size(); i++)
	{
		if (ents[order[i]].components[int(ComponentType::COMPONENT_RENDER_PIPELINE)] != -1)
		{
			//debugHandler->Log(std::string(std::string("Attempting to render shape ") + std::to_string(ents[order[i]].components[int(ComponentType::COMPONENT_SHAPE)])).c_str());
			auto shape = static_cast<SGShapeManagerSFML*>(shapeManager)->GetShape(ents[order[i]].components[int(ComponentType::COMPONENT_SHAPE)]);
			shape->setPosition(ents[order[i]].xPos, ents[order[i]].yPos);
			shape->setRotation(ents[order[i]].rotation * 180 / 3.1459);

			if (currentPipeline != ents[order[i]].components[int(ComponentType::COMPONENT_RENDER_PIPELINE)])
			{
				SetRenderPipeline(ents[order[i]].components[int(ComponentType::COMPONENT_RENDER_PIPELINE)]);
				currentPipeline = ents[order[i]].components[int(ComponentType::COMPONENT_RENDER_PIPELINE)];
			}

			SetPrivateBindings(ents[order[i]], sceneId);
			static_cast<SGShaderHandler2DSFML*>(shaderManager)->SetHandlerBindings(ents[order[i]], static_cast<SGTextureManager2DSFML*>(textureManager), static_cast<SGShapeManagerSFML*>(shapeManager), ents[order[i]].components[int(ComponentType::COMPONENT_RENDER_PIPELINE)]);
			window->draw(*shape, static_cast<SGShaderHandler2DSFML*>(shaderManager)->GetShader(ents[order[i]].components[int(ComponentType::COMPONENT_RENDER_PIPELINE)]));
		}
		else
		{
			if (ents[order[i]].components[int(ComponentType::COMPONENT_SHAPE)] != -1)
			{
				auto shape = static_cast<SGShapeManagerSFML*>(shapeManager)->GetShape(ents[order[i]].components[int(ComponentType::COMPONENT_SHAPE)]);
				shape->setPosition(ents[order[i]].xPos, ents[order[i]].yPos);
				shape->setRotation(ents[order[i]].rotation * 180 / 3.1459);

				if (ents[order[i]].components[int(ComponentType::COMPONENT_TEXTURE)] != -1 && shape->getTexture() != static_cast<SGTextureManager2DSFML*>(textureManager)->GetTexture(ents[order[i]].components[int(ComponentType::COMPONENT_TEXTURE)]))
				{
					shape->setTexture(static_cast<SGTextureManager2DSFML*>(textureManager)->GetTexture(ents[order[i]].components[int(ComponentType::COMPONENT_TEXTURE)]));
				}

				window->draw(*shape);
			}
		}
		
	}

	for (int i = 0; i < order.size(); i++)
	{
		if (ents[order[i]].components[int(ComponentType::COMPONENT_TEXT)] != -1)
		{
			auto text = static_cast<SGTextManager2DSFML*>(textManager)->GetText(ents[order[i]].components[int(ComponentType::COMPONENT_TEXT)]);
			text->text.setPosition(ents[order[i]].xPos + text->xOffset, ents[order[i]].yPos + text->yOffset);
			text->text.setRotation(ents[order[i]].rotation * 180 / 3.1459);
			window->draw(text->text);
		}
	}


	//debugHandler->Log("Finished rendering scene");
}

void SG::SGRendererSFML::Display()
{
	window->display();
}

uint8_t * SG::SGRendererSFML::GetRenderTargetData()
{
	return nullptr;
}

void SG::SGRendererSFML::SetRenderTargetData(uint8_t * data)
{
}

SG::SGWindow SG::SGRendererSFML::GetWindow()
{
	SGWindow toReturn;
	toReturn.window = window;
	return toReturn;
}

SG::SGPoint SG::SGRendererSFML::PixelToPosition(SGPoint& pixelToTranslate)
{
	auto pos = window->mapPixelToCoords(sf::Vector2i(int(pixelToTranslate.xPos), int(pixelToTranslate.yPos)));
	SGPoint toReturn;
	toReturn.xPos = pos.x;
	toReturn.yPos = pos.y;

	return toReturn;
}

SG::SGPoint SG::SGRendererSFML::PositionToPixel(SGPoint& pointToTranslate)
{
	auto pos = window->mapCoordsToPixel(sf::Vector2f(pointToTranslate.xPos, pointToTranslate.yPos));
	SGPoint toReturn;
	toReturn.xPos = pos.x;
	toReturn.yPos = pos.y;

	return toReturn;
}

SG::SGMatrix4x4 SG::SGRendererSFML::GetProjectionMatrix(SGCamera2D & camera)
{
	SGMatrix4x4 temp;
	auto matrix = sf::View(sf::Vector2f(camera.xPos, camera.yPos), sf::Vector2f(camera.width, camera.height)).getTransform().getMatrix();
	memcpy(temp.elements, matrix, 16 * sizeof(float));
	return temp;
}
