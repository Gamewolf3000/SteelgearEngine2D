#pragma once

#include "SGEntity2D.h"
#include "SGBackEndData.h"
#include "EventManager.h"
#include "SGVector.h"
#include "DebugMessageHandler.h"

#include <functional>

namespace SG
{
	template<typename T>
	class SGManager
	{
	protected:
		SGVector<T> objects;
		EventManager<BackEndEvent>* eventHandler;
		const ComponentType componentType;
		DebugMessageHandler* debugHandler;

		void AddComponentToEntity(SGEntity2DHandle& ent, int spotInManager);
		void RemoveComponentFromEntity(SGEntity2DHandle& ent);
		void RemoveComponentFromEntity(SGEntity2D* ent);

	public:
		SGManager<T>(unsigned int startNr, unsigned int expandAmount, EventManager<BackEndEvent>* eventsBackEnd, ComponentType typeOfComponent, bool AddRemoveToEvent = true);
		virtual ~SGManager();

		virtual void Remove(SGEntity2DHandle* ent) = 0;

	};

	template<typename T>
	inline SGManager<T>::SGManager(unsigned int startNr, unsigned int expandAmount, EventManager<BackEndEvent>* eventsBackEnd, ComponentType typeOfComponent, bool AddRemoveToEvent) : objects(startNr, expandAmount), eventHandler(eventsBackEnd), componentType(typeOfComponent)
	{
		if (AddRemoveToEvent)
		{
			std::function<void(SGEntity2DHandle*)> f = std::bind(&SGManager::Remove, this, std::placeholders::_1);
			eventHandler->BindFunctionToEvent(SG::BackEndEvent::ENTITY_DESTROYED, f);

		}

		debugHandler = DebugMessageHandler::GetInstance();
	}
	
	template<typename T>
	inline SGManager<T>::~SGManager()
	{
		// Empty
	}

	template<typename T>
	inline void SGManager<T>::AddComponentToEntity(SGEntity2DHandle& ent, int spotInManager)
	{
		SGEntity2D* temp = nullptr;
		eventHandler->TriggerEvent(BackEndEvent::GET_ENTITY, &temp, &ent);

		if (temp->components[int(componentType)] != -1)
			Remove(&ent);

		temp->components[int(componentType)] = spotInManager;
		objects.IncrementReferenceCountOfElement(spotInManager);
	}

	template<typename T>
	inline void SGManager<T>::RemoveComponentFromEntity(SGEntity2DHandle& ent)
	{
		SGEntity2D* temp = nullptr;
		eventHandler->TriggerEvent(BackEndEvent::GET_ENTITY, &temp, &ent);

		if (temp->components[int(componentType)] != -1)
			Remove(&ent);

		temp->components[int(componentType)] = -1;
	}

	template<typename T>
	inline void SGManager<T>::RemoveComponentFromEntity(SGEntity2D* ent)
	{
		ent->components[int(componentType)] = -1;
	}


}