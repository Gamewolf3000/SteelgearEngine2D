#pragma once

#include "SGGuid.h"
#include "SGVector.h"

namespace SG
{

	enum class BackEndEvent
	{
		GET_ENTITY,
		GET_TRANSFORM,
		ENTITY_DESTROYED,
		TRANSFORM_REQUEST,
		SET_DIRTY_SCENE
	};


	enum class ComponentType
	{
		COMPONENT_ENTITY,
		COMPONENT_SHAPE,
		COMPONENT_TEXTURE,
		COMPONENT_TRANSFORM,
		COMPONENT_RENDER_PIPELINE,
		COMPONENT_TEXT,
		NR_OF_COMPONENT_TYPES
	};

	struct SGGraphicSettings2D
	{
		unsigned int screenWidth = 1280;
		unsigned int screenHeight = 720;

		std::string windowTitle = "Steelgear2D";
		bool threadedRendering = false;
	};

	struct SGSettings2D
	{
		SGGraphicSettings2D graphicsSettings;

		SGSettings2D() {};
	};

}