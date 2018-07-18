#pragma once

#include "../SG2DUtility/Headers/SGManager.h"
#include "../SG2DUtility/Headers/SGMath.h"

namespace SG
{
	struct ParentChildRelation
	{
		std::vector<SGEntity2DHandle> parents;
		std::vector<SGEntity2DHandle> children;

		bool operator==(const ParentChildRelation& other) const
		{
			return false;
		}
	};

	class SGTransformManager2D : public SGManager<ParentChildRelation>
	{
	private:


	public:
		SGTransformManager2D(unsigned int startNr, unsigned int expandAmount, EventManager<BackEndEvent>* eventsBackEnd);
		~SGTransformManager2D();

		void CreateParentChildRelation(SGEntity2DHandle& ent);

		void Move(SGEntity2DHandle& ent, double xMovement, double yMovement);
		void SetPos(SGEntity2DHandle& ent, double xPos, double yPos);
		void Rotate(SGEntity2DHandle& ent, double rotationAmount);
		void SetRotation(SGEntity2DHandle& ent, double rotation);

		void BindChild(SGEntity2DHandle& parent, SGEntity2DHandle& child);
		void RemoveChild(SGEntity2DHandle& parent, SGEntity2DHandle& child);
		void RemoveAllChildren(SGEntity2DHandle& ent);
		void RemoveAllParents(SGEntity2DHandle& ent);

		SGPoint GetPos(SGEntity2DHandle& ent);

		void Remove(SGEntity2DHandle* ent) override;
	};
}