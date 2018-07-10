#pragma once

#include "SGManager.h"
#include "SGEntity2D.h"
#include "SGBackEndData.h"

namespace SG
{
	class SGEntityManager2D : public SGManager<SGEntity2D>
	{
		friend class SGRenderer2D;
		friend class SGScene2D;

	private:
		SGGuid sceneGuid;
		EventManager<BackEndEvent>::EventHandle getEntityEventHandle;
		EventManager<BackEndEvent>::EventHandle setDirtyEventHandle;
		std::vector<unsigned int> sortedEntities;
		bool isDirty = true;

		void SortEntities();
		void InsertionSort(unsigned int startPos, unsigned int nrOfElements, unsigned short levelsSorted);

	public:
		SGEntityManager2D(unsigned int startNrOfEntities, unsigned int entitiesExpandAmount, EventManager<BackEndEvent>* eventsBackEnd, SGGuid sceneIdentifier);
		~SGEntityManager2D();

		const SGEntity2DHandle CreateEntity();

		void Remove(SGEntity2DHandle* ent) override;
	};
}