#pragma once

#include "SGManager.h"
#include "SGShapeManager.h"
#include <SFML\Graphics.hpp>

namespace SG
{
	class SGShapeManagerSFML : public SGManager<sf::Shape*>, public SGShapeManager
	{
	private:
		void CleanUp() override;

	public:
		SGShapeManagerSFML(unsigned int startNr, unsigned int expandAmount, EventManager<BackEndEvent>* eventsBackEnd);
		~SGShapeManagerSFML();

		void CreateRectangleShape(SGEntity2DHandle& ent, float rectWidth, float rectHeight) override;
		void CreateCircleShape(SGEntity2DHandle& ent, float circleRadius, unsigned int nrOfPoints) override;

		SGRect GetShapeRect(SGEntity2DHandle& ent) override;

		virtual void Remove(SGEntity2DHandle* ent) override;

		inline sf::Shape* GetShape(int index)
		{
			return objects[index];
		}
	};
}