#pragma once
#include "SGInputHandler.h"
#include <vector>

namespace SG
{
	class SGInputHandlerSFML : public SGInputHandler
	{
	private:

		void UpdateMouseInput(SGWindow& window) override;
		std::vector<int> keyLookup;

	public:

		SGInputHandlerSFML();
		~SGInputHandlerSFML();

		void UpdateInput(SGWindow& window) override;
		float GetControllerAxisValue(unsigned short controllerNumber, ControllerAxis axis) override;

	};


}