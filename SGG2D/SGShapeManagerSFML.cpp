#include "SGShapeManagerSFML.h"

void SG::SGShapeManagerSFML::CleanUp()
{
	mutexLock.lock();
	
	for (auto& index : toClean)
	{
		if (objects.GetReferenceCountOfElement(index) == 1)
		{
			delete objects[index];
			objects[index] = nullptr;
		}

		objects.Remove(index);
		//debugHandler->StoreMessage(std::string(std::string("Removed ") + std::to_string(index)).c_str());
	}

	toClean.clear();

	mutexLock.unlock();
}

SG::SGShapeManagerSFML::SGShapeManagerSFML(unsigned int startNr, unsigned int expandAmount, EventManager<BackEndEvent>* eventsBackEnd) : SGManager(startNr, expandAmount, eventsBackEnd, ComponentType::COMPONENT_SHAPE)
{

}

SG::SGShapeManagerSFML::~SGShapeManagerSFML()
{
	for (int i = 0; i < objects.Size(); i++)
	{
		delete objects[i];
	}
}

void SG::SGShapeManagerSFML::CreateRectangleShape(SGEntity2DHandle& ent, float rectWidth, float rectHeight)
{
	sf::Shape* shape = new sf::RectangleShape(sf::Vector2f(rectWidth, rectHeight));
	shape->setOrigin(rectWidth / 2, rectHeight / 2);
	shape->setTextureRect(sf::IntRect(0, 0, 1, 1));

	auto spot = this->objects.Add(shape);
	this->AddComponentToEntity(ent, spot);
	//debugHandler->StoreMessage(std::string(std::string("Added ") + std::to_string(spot)).c_str());
}

void SG::SGShapeManagerSFML::CreateCircleShape(SGEntity2DHandle& ent, float circleRadius, unsigned int nrOfPoints)
{
	sf::Shape* shape = new sf::CircleShape(circleRadius, nrOfPoints);
	shape->setTextureRect(sf::IntRect(0, 0, 1, 1));

	this->AddComponentToEntity(ent, this->objects.Add(shape));
}

SG::SGRect SG::SGShapeManagerSFML::GetShapeRect(SGEntity2DHandle& ent)
{
	SGEntity2D* temp = nullptr;
	eventHandler->TriggerEvent(SG::BackEndEvent::GET_ENTITY, &temp, &ent);

	if (temp->components[int(componentType)] != -1)
	{
		auto shapeRect = objects[temp->components[int(componentType)]]->getLocalBounds();
		SGRect toReturn;

		toReturn.leftMost = shapeRect.left + temp->xPos;
		toReturn.topMost = shapeRect.top + temp->yPos;
		toReturn.width = shapeRect.width;
		toReturn.height = shapeRect.height;

		return toReturn;
	}

	throw "Shape not found when requesting rect";
}

void SG::SGShapeManagerSFML::Remove(SGEntity2DHandle* ent)
{
	SGEntity2D* temp = nullptr;
	eventHandler->TriggerEvent(SG::BackEndEvent::GET_ENTITY, &temp, ent);

	if (temp->components[int(componentType)] != -1)
	{

		mutexLock.lock();
		toCleanBuffer.push_back(temp->components[int(componentType)]);
		mutexLock.unlock();

		this->RemoveComponentFromEntity(temp);

	}
}
