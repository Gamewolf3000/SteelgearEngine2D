#pragma once

#include <vector>
#include <unordered_map>
#include "../SG2DUtility/Headers/SGMath.h"
#include "../SGG2D/SGRenderer2D.h"

typedef int SGInputType;

namespace SG
{

	enum class ControllerAxis
	{
		AXIS_1,
		AXIS_2,
		AXIS_3,
		AXIS_4,
		AXIS_5,
		AXIS_6,
		AXIS_7,
		AXIS_8,
		AXIS_9,
		AXIS_10
	};

	enum class InputStatus
	{
		UP,
		PRESSED,
		DOWN,
		RELEASED
	};

	enum class KeyboardInput
	{
		KEY_A,
		KEY_B,
		KEY_C,
		KEY_D,
		KEY_E,
		KEY_F,
		KEY_G,
		KEY_H,
		KEY_I,
		KEY_J,
		KEY_K,
		KEY_L,
		KEY_M,
		KEY_N,
		KEY_O,
		KEY_P,
		KEY_Q,
		KEY_R,
		KEY_S,
		KEY_T,
		KEY_U,
		KEY_V,
		KEY_W,
		KEY_X,
		KEY_Y,
		KEY_Z,
		KEY_0,
		KEY_1,
		KEY_2,
		KEY_3,
		KEY_4,
		KEY_5,
		KEY_6,
		KEY_7,
		KEY_8,
		KEY_9,
		KEY_LSHIFT,
		KEY_LCTRL,
		KEY_ESC,
		KEY_LEFT,
		KEY_UP,
		KEY_RIGHT,
		KEY_DOWN
	};

	enum class ControllerInput
	{
		BUTTON_1,
		BUTTON_2,
		BUTTON_3,
		BUTTON_4,
		BUTTON_5,
		BUTTON_6,
		BUTTON_7,
		BUTTON_8,
		BUTTON_9,
		BUTTON_10,
		BUTTON_11,
		BUTTON_12,
		BUTTON_13,
		BUTTON_14,
		BUTTON_15,
		BUTTON_16,
		BUTTON_17,
		BUTTON_18,
		BUTTON_19,
		BUTTON_20,
	};

	class SGInputHandler
	{

	protected:
		
		struct ButtonSource
		{
			ControllerInput button;
			unsigned short controllerNumber;
		};

		struct InputSource
		{

			SGInputType identifier;
			std::vector<KeyboardInput> mappedKeys;
			std::vector<ButtonSource> mappedButtons;
			InputStatus inputStatus = InputStatus::UP;

		};

		SGPoint currentMousePos;
		float deadzoneLimit = 0.1f;

		std::unordered_map<SGInputType, InputSource> sources;

		virtual void UpdateMouseInput(SGWindow& window) = 0;

	public:

		SGInputHandler()
		{

		}

		virtual ~SGInputHandler()
		{
		}

		void AddSource(SGInputType inputIdentifier)
		{

			typename std::unordered_map<SGInputType, InputSource>::const_iterator finder = sources.find(inputIdentifier);

			if (finder == sources.end())
			{
				InputSource temp;
				temp.identifier = inputIdentifier;

				sources[inputIdentifier] = temp;
			}

		}

		bool BindKey(SGInputType idToBindTo, KeyboardInput key)
		{
			typename std::unordered_map<SGInputType, InputSource>::const_iterator finder = sources.find(idToBindTo);

			if (finder == sources.end())
			{
				return false;
			}
			else
			{
				for (int i = 0; i < sources[idToBindTo].mappedKeys.size(); i++)
				{
					if (sources[idToBindTo].mappedKeys[i] == key)
					{
						return true;
					}
				}

				sources[idToBindTo].mappedKeys.push_back(key);

				return true;
			}
		}

		bool BindButton(SGInputType idToBindTo, ControllerInput button, unsigned short controllerNumber)
		{
			typename std::unordered_map<SGInputType, InputSource>::const_iterator finder = sources.find(idToBindTo);

			if (finder == sources.end())
			{
				return false;
			}
			else
			{
				for (int i = 0; i < sources[idToBindTo].mappedButtons.size(); i++)
				{
					if (sources[idToBindTo].mappedButtons[i].button == button && sources[idToBindTo].mappedButtons[i].controllerNumber == controllerNumber)
					{
						return true;
					}
				}

				ButtonSource temp;
				temp.button = button;
				temp.controllerNumber = controllerNumber;

				sources[idToBindTo].mappedButtons.push_back(temp);

				return true;
			}
		}

		virtual void UpdateInput(SGWindow& window) = 0;

		inline bool IsPressed(SGInputType identifier)
		{
			if (sources[identifier].inputStatus == InputStatus::PRESSED)
				return true;
			else
				return false;
		}

		inline bool IsDown(SGInputType identifier)
		{
			if (sources[identifier].inputStatus == InputStatus::DOWN || sources[identifier].inputStatus == InputStatus::PRESSED)
				return true;
			else
				return false;
		}

		inline bool IsReleased(SGInputType identifier)
		{
			if (sources[identifier].inputStatus == InputStatus::RELEASED)
				return true;
			else
				return false;
		}

		inline bool IsUp(SGInputType identifier)
		{
			if (sources[identifier].inputStatus == InputStatus::UP || sources[identifier].inputStatus == InputStatus::RELEASED)
				return true;
			else
				return false;
		}

		inline void SetButton(SGInputType identifier, InputStatus value)
		{
			sources[identifier].inputStatus = value;
		}

		inline SGPoint GetMousePos()
		{
			return currentMousePos;
		}

		inline void SetMousePos(int xPos, int yPos)
		{
			currentMousePos.xPos = xPos;
			currentMousePos.yPos = yPos;
		}

		inline void SetMousePos(SGPoint& pos)
		{
			currentMousePos = pos;
		}

		void SetDeadzoneLimit(float value)
		{
			deadzoneLimit = value;
		}

		inline virtual float GetControllerAxisValue(unsigned short controllerNumber, ControllerAxis axis) = 0;
	};

}