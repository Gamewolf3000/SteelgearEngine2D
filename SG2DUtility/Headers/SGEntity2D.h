#pragma once

#include <functional>
#include "SGBackEndData.h"
#include "SGGuid.h"

namespace SG
{
	struct SGEntity2D
	{
	public:
		//template<typename T> friend class SGManager;
		//friend class SGEntityManager2D;

		double xPos = 0;
		double yPos = 0;
		double rotation = 0;

		unsigned int id = -1; // -1 give the max value
		//std::unordered_map<SGGuid, int, SGGuid> components;
		int components[int(ComponentType::NR_OF_COMPONENT_TYPES) + 1];

		SGEntity2D()
		{
			std::fill_n(components, int(ComponentType::NR_OF_COMPONENT_TYPES) + 1, -1);
		}

		bool operator==(const SGEntity2D& other) const
		{
			return this->id == other.id;
		}

	};

	struct SGEntity2DHandle
	{
	private:
		template<typename T> friend class SGManager;
		friend class SGEntityManager2D;
		friend class SGRenderer2D;

		unsigned int entityPosition;
		SGGuid handlerID;

		SGEntity2DHandle(unsigned int position, SGGuid idOfHandler) : entityPosition(position), handlerID(idOfHandler)
		{

		}

	public:

		SGEntity2DHandle()
		{
			handlerID = SGGuid();
			entityPosition = -1;
		}

		bool operator==(const SGEntity2DHandle& other) const
		{
			return this->entityPosition == other.entityPosition && this->handlerID == other.handlerID;
		}

		std::size_t operator()(const SGEntity2DHandle& other) const
		{
			return entityPosition;
		}
	};

	struct SGSceneEntities
	{
	private:
		friend class SGScene2D;
		friend class SGRenderer2D;

		SGVector<SGEntity2D> entities;
		std::vector<unsigned int> sorting;
		SGGuid handlerID;

		SGSceneEntities(const SGVector<SGEntity2D>& entitiesInScene, SGGuid& handlerIdentifier, std::vector<unsigned int>& sorted) : entities(entitiesInScene), handlerID(handlerIdentifier), sorting(sorted)
		{

		}

	public:

	};
}