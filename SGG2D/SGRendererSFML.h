#pragma once

#include "SGRenderer2D.h"

#include <vector>

namespace sf
{
	class RenderWindow;
}

namespace SG
{

	class SGRendererSFML : public SGRenderer2D
	{
	private:
		sf::RenderWindow* window;

		void RenderThread() override;

		void HandleWindowEvents() override;

	public:
		SGRendererSFML(SGGraphicSettings2D gSettings, EventManager<BackEndEvent>* eventsBackEnd);
		~SGRendererSFML();

		void ClearRenderTarget(const SGColour& clearColour) override;
		void RenderScene(const SGSceneEntities& sceneEntities, const SGCamera2D& camera, const SGViewPort2D& viewPort) override;
		void Display() override;
		uint8_t* GetRenderTargetData() override;
		void SetRenderTargetData(uint8_t* data) override;
		SGRect GetWindowRect() override;

		SGMatrix4x4 GetProjectionMatrix(SGCamera2D& camera) override;
	};
}