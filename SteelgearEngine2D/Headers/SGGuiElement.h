#pragma once

#include "../../SG2DUtility/Headers/SGGuid.h"
#include "../SGScripts/SGScriptManagerAS.h"
#include "./../SG2DUtility/Headers/SGEntity2D.h"
#include "../../SGG2D/SGRenderer2D.h"

namespace SG
{
	struct SGGuiSettings
	{
		SG::SGGuid textureNotSelected;
		SG::SGGuid textureSelected;
		SG::SGGuid pipeline;
		SG::SGTextSettings text;
		std::string scriptPath;
		std::string updateFunctionName;
		std::string selectedFunctionName;
		float xSize;
		float ySize;
		float xPos;
		float yPos;
	};

	class SGGuiElement
	{
		friend class SGEngine2D;

	private:
		SG::SGEntity2DHandle ent;
		SG::SGGuid textureNotSelected;
		SG::SGGuid textureSelected;
		bool isCurrentlySelected = false;
		bool changed = false;
		SG::SGScriptHandle script;
		std::string updateFunctionName;
		std::string selectedFunctionName;

		SGGuiElement(SG::SGEntity2DHandle& entToUse)
		{
			ent = entToUse;
		}

	public:
		SGGuiElement()
		{

		}

		template<typename... ScriptArgs>
		void Update(SG::SGRenderer2D* graphics, SG::SGScriptManagerAS* scriptManager, ScriptArgs&&... parameters)
		{
			if (changed && textureSelected != SG::SGGuid(""))
			{
				if (isCurrentlySelected)
					graphics->TextureManager()->SetTexture(ent, textureSelected);
				else
					graphics->TextureManager()->SetTexture(ent, textureNotSelected);

				changed = false;
			}

			if(updateFunctionName.length() > 0)
				scriptManager->ExecuteScript(script, updateFunctionName.c_str(), parameters...);
		}

		void SetSelected(bool value)
		{
			if (isCurrentlySelected == value)
				return;

			isCurrentlySelected = value;
			changed = true;
		}

		template<typename... ScriptArgs>
		void RunSelectionFunction(SG::SGScriptManagerAS* scriptManager, ScriptArgs&&... parameters)
		{
			if(selectedFunctionName.length() > 0)
				scriptManager->ExecuteScript(script, selectedFunctionName.c_str(), parameters...);
		}

		inline SG::SGEntity2DHandle& GetEntity()
		{
			return ent;
		}

	};
}