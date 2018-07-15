#pragma once
#include "SGInputHandler.h"
#include <vector>

namespace SG
{
	class SGInputHandlerSFML : public SGInputHandler
	{
	private:

		void UpdateMouseInput(SGRect& windowRect) override;
		std::vector<int> keyLookup;

	public:

		SGInputHandlerSFML();
		~SGInputHandlerSFML();

		void UpdateInput(SGRect& windowRect) override;
		float GetControllerRightStickValue(unsigned short controllerNumber, AxisDirection dir) override;
		float GetControllerLeftStickValue(unsigned short controllerNumber, AxisDirection dir) override;

	};


}