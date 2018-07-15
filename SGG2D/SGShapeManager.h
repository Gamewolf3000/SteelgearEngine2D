#pragma once

#include "SGG2DUtility.h"
#include <mutex>

namespace SG
{
	enum class SGShapeType
	{
		RECTANGLE,
		CIRCLE,
		CONVEX_POLYGON
	};

	class SGShapeManager
	{
		friend class SGRenderer2D;

	protected:
		std::vector<int> toClean;
		std::vector<int> toCleanBuffer;
		std::mutex mutexLock;

		virtual void CleanUp() = 0;
		void UpdateData();

	public:
		SGShapeManager();
		virtual ~SGShapeManager();

		virtual void CreateRectangleShape(SGEntity2DHandle& ent, float rectWidth, float rectHeight) = 0;
		virtual void CreateCircleShape(SGEntity2DHandle& ent, float circleRadius, unsigned int nrOfPoints) = 0;

		virtual SGRect GetShapeRect(SGEntity2DHandle& ent) = 0;

	};
}