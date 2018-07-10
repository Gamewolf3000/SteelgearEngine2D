#include "../Headers/SGEntityManager2D.h"

void SG::SGEntityManager2D::SortEntities()
{
	//sortedEntities.clear();
	//for(int i = 0; i < objects.Size(); i++)
	//{
	//	sortedEntities.push_back(i);
	//}
	//return;

	if (isDirty)
	{
		//for (int i = 0; i < sortedEntities.size(); i++)
		//{
		//	if (objects.GetReferenceCountOfElement(sortedEntities[i]) == 0)
		//	{
		//		sortedEntities[i] = sortedEntities.back();
		//		sortedEntities.pop_back();
		//		i--;
		//	}
		//}

		InsertionSort(0, sortedEntities.size(), 0);
		isDirty = false;
	}
}

void SG::SGEntityManager2D::InsertionSort(unsigned int startPos, unsigned int nrOfElements, unsigned short levelsSorted)
{
	const int maxLevel = 1;

	// zero or one element in list
	if (nrOfElements <= 1)
		return;

	int componentIndex;

	switch (levelsSorted)
	{
	default:
		return;
	case 0:
		componentIndex = int(SG::ComponentType::COMPONENT_RENDER_PIPELINE);
		break;
	case 1:
		componentIndex = int(SG::ComponentType::COMPONENT_TEXTURE);
		break;
	}

	SGEntity2D temp;
	int j = startPos;

	for (int i = startPos + 1; i < startPos + nrOfElements; i++)
	{
		temp = objects[sortedEntities[i]];
		j = i - 1;

		while (j >= 0 && objects[sortedEntities[j]].components[componentIndex] > temp.components[componentIndex])
		{
			sortedEntities[j + 1] = sortedEntities[j];
			j--;
		}
		sortedEntities[j + 1] = temp.id;

	}

	if (levelsSorted >= maxLevel)
		return;

	int counter = 1;
	int newStartPos = startPos;
	int currentComponent = objects[sortedEntities[startPos]].components[componentIndex];

	for (int i = startPos + 1; i < startPos + nrOfElements; i++)
	{
		if (objects[sortedEntities[newStartPos + counter]].components[componentIndex] != currentComponent)
		{
			currentComponent = objects[sortedEntities[newStartPos + counter]].components[componentIndex];
			InsertionSort(newStartPos, counter, levelsSorted + 1);
			newStartPos += counter;
			counter = 0;
		}

		counter++;
	}

	InsertionSort(newStartPos, counter, levelsSorted + 1);
}

SG::SGEntityManager2D::SGEntityManager2D(unsigned int startNrOfEntities, unsigned int entitiesExpandAmount, EventManager<BackEndEvent>* eventsBackEnd, SGGuid sceneIdentifier) : SGManager(startNrOfEntities, entitiesExpandAmount, eventsBackEnd, ComponentType::COMPONENT_ENTITY, false), sceneGuid(sceneIdentifier)
{
	auto getEvent = [this](SGEntity2D** ptr, SGEntity2DHandle* toGet)
	{
		if (sceneGuid == toGet->handlerID)
		{
			*ptr = &objects[toGet->entityPosition];
		}
	};

	getEntityEventHandle = eventHandler->BindFunctionToEvent<SGEntity2D**, SGEntity2DHandle*>(BackEndEvent::GET_ENTITY, getEvent);

	auto dirtyEvent = [this](SGEntity2DHandle* ent)
	{
		if (sceneGuid == ent->handlerID)
		{
			isDirty = true;
		}
	};

	setDirtyEventHandle = eventHandler->BindFunctionToEvent<SGEntity2DHandle*>(BackEndEvent::SET_DIRTY_SCENE, dirtyEvent);
}

SG::SGEntityManager2D::~SGEntityManager2D()
{
	eventHandler->RemoveEvent(getEntityEventHandle);
	eventHandler->RemoveEvent(setDirtyEventHandle);
}

const SG::SGEntity2DHandle SG::SGEntityManager2D::CreateEntity()
{
	SGEntity2D temp;
	unsigned int id = objects.Add(temp);
	objects[id].id = id;
	objects.IncrementReferenceCountOfElement(id);

	SGEntity2DHandle toReturn(id, this->sceneGuid);
	isDirty = true; // we have added an entity and thus need to sort;
	sortedEntities.push_back(id); // LÄGG TILL I ALLA RELEVANTA HANDLERS SÅ DE SÄGER DIRTY PÅ DEN


	return toReturn;
}

void SG::SGEntityManager2D::Remove(SGEntity2DHandle* ent)
{
	if (ent->handlerID == this->sceneGuid) // check so that the entity belongs to this scene
	{
		eventHandler->TriggerEvent(SG::BackEndEvent::ENTITY_DESTROYED, ent); // tell all handlers to remove their properties if this entity have any

		objects.Remove(ent->entityPosition);

		ent->handlerID = SGGuid(""); // to prevent it from being removed again

		isDirty = true; // we have removed an entity and thus need to sort;

		for (auto& entity : sortedEntities)
		{
			if (entity == ent->entityPosition)
			{
				entity = sortedEntities.back();
				sortedEntities.pop_back();
				return;
			}
		}

		return; // if this happened then something has gone wrong
	}
}
