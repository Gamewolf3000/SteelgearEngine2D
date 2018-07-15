#include "SGInputHandlerSFML.h"
#include "../SGInput/SFML/include/SFML/Window.hpp"

SG::SGInputHandlerSFML::SGInputHandlerSFML()
{
	keyLookup.resize(100);
	keyLookup[int(KeyboardInput::KEY_A)] = sf::Keyboard::Key::A;
	keyLookup[int(KeyboardInput::KEY_B)] = sf::Keyboard::Key::B;
	keyLookup[int(KeyboardInput::KEY_C)] = sf::Keyboard::Key::C;
	keyLookup[int(KeyboardInput::KEY_D)] = sf::Keyboard::Key::D;
	keyLookup[int(KeyboardInput::KEY_E)] = sf::Keyboard::Key::E;
	keyLookup[int(KeyboardInput::KEY_F)] = sf::Keyboard::Key::F;
	keyLookup[int(KeyboardInput::KEY_G)] = sf::Keyboard::Key::G;
	keyLookup[int(KeyboardInput::KEY_H)] = sf::Keyboard::Key::H;
	keyLookup[int(KeyboardInput::KEY_I)] = sf::Keyboard::Key::I;
	keyLookup[int(KeyboardInput::KEY_J)] = sf::Keyboard::Key::J;
	keyLookup[int(KeyboardInput::KEY_K)] = sf::Keyboard::Key::K;
	keyLookup[int(KeyboardInput::KEY_L)] = sf::Keyboard::Key::L;
	keyLookup[int(KeyboardInput::KEY_M)] = sf::Keyboard::Key::M;
	keyLookup[int(KeyboardInput::KEY_N)] = sf::Keyboard::Key::N;
	keyLookup[int(KeyboardInput::KEY_O)] = sf::Keyboard::Key::O;
	keyLookup[int(KeyboardInput::KEY_P)] = sf::Keyboard::Key::P;
	keyLookup[int(KeyboardInput::KEY_Q)] = sf::Keyboard::Key::Q;
	keyLookup[int(KeyboardInput::KEY_R)] = sf::Keyboard::Key::R;
	keyLookup[int(KeyboardInput::KEY_S)] = sf::Keyboard::Key::S;
	keyLookup[int(KeyboardInput::KEY_T)] = sf::Keyboard::Key::T;
	keyLookup[int(KeyboardInput::KEY_U)] = sf::Keyboard::Key::U;
	keyLookup[int(KeyboardInput::KEY_V)] = sf::Keyboard::Key::V;
	keyLookup[int(KeyboardInput::KEY_W)] = sf::Keyboard::Key::W;
	keyLookup[int(KeyboardInput::KEY_X)] = sf::Keyboard::Key::X;
	keyLookup[int(KeyboardInput::KEY_Y)] = sf::Keyboard::Key::Y;
	keyLookup[int(KeyboardInput::KEY_Z)] = sf::Keyboard::Key::Z;
	keyLookup[int(KeyboardInput::KEY_0)] = sf::Keyboard::Key::Num0;
	keyLookup[int(KeyboardInput::KEY_1)] = sf::Keyboard::Key::Num1;
	keyLookup[int(KeyboardInput::KEY_2)] = sf::Keyboard::Key::Num2;
	keyLookup[int(KeyboardInput::KEY_3)] = sf::Keyboard::Key::Num3;
	keyLookup[int(KeyboardInput::KEY_4)] = sf::Keyboard::Key::Num4;
	keyLookup[int(KeyboardInput::KEY_5)] = sf::Keyboard::Key::Num5;
	keyLookup[int(KeyboardInput::KEY_6)] = sf::Keyboard::Key::Num6;
	keyLookup[int(KeyboardInput::KEY_7)] = sf::Keyboard::Key::Num7;
	keyLookup[int(KeyboardInput::KEY_8)] = sf::Keyboard::Key::Num8;
	keyLookup[int(KeyboardInput::KEY_9)] = sf::Keyboard::Key::Num9;
	keyLookup[int(KeyboardInput::KEY_LSHIFT)] = sf::Keyboard::Key::LShift;
	keyLookup[int(KeyboardInput::KEY_LCTRL)] = sf::Keyboard::Key::LControl;
	keyLookup[int(KeyboardInput::KEY_ESC)] = sf::Keyboard::Key::Escape;
	keyLookup[int(KeyboardInput::KEY_LEFT)] = sf::Keyboard::Key::Left;
	keyLookup[int(KeyboardInput::KEY_UP)] = sf::Keyboard::Key::Up;
	keyLookup[int(KeyboardInput::KEY_RIGHT)] = sf::Keyboard::Key::Right;
	keyLookup[int(KeyboardInput::KEY_DOWN)] = sf::Keyboard::Key::Down;

}

SG::SGInputHandlerSFML::~SGInputHandlerSFML()
{

}

void SG::SGInputHandlerSFML::UpdateMouseInput(SGRect& windowRect)
{
	auto pos = sf::Mouse::getPosition();
	currentMousePos.xPos = pos.x - windowRect.leftMost;
	currentMousePos.yPos = pos.y - windowRect.topMost;

}

void SG::SGInputHandlerSFML::UpdateInput(SGRect& windowRect)
{
	UpdateMouseInput(windowRect);

	for (auto& source : sources)
	{
		bool found = false;

		for (int i = 0; i < source.second.mappedKeys.size(); i++)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(keyLookup[int(source.second.mappedKeys[i])])))
			{
				found = true;

				if (source.second.inputStatus == InputStatus::UP)
					source.second.inputStatus = InputStatus::PRESSED;
				else
					source.second.inputStatus = InputStatus::DOWN;

				break;
			}
		}

		if (!found)
			source.second.inputStatus = InputStatus::UP;
	}
}

float SG::SGInputHandlerSFML::GetControllerRightStickValue(unsigned short controllerNumber, AxisDirection dir)
{
	return 0.0f;
}

float SG::SGInputHandlerSFML::GetControllerLeftStickValue(unsigned short controllerNumber, AxisDirection dir)
{
	return 0.0f;
}