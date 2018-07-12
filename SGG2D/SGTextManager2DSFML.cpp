#include "SGTextManager2DSFML.h"

void SG::SGTextManager2DSFML::CleanUp()
{
	mutexLock.lock();

	for (auto& index : toClean)
	{
		objects.Remove(index);
	}

	toClean.clear();

	mutexLock.unlock();
}

SG::SGTextManager2DSFML::SGTextManager2DSFML(unsigned int startNr, unsigned int expandAmount, EventManager<BackEndEvent>* eventsBackEnd) : SGManager(startNr, expandAmount, eventsBackEnd, ComponentType::COMPONENT_TEXT)
{
	
}

SG::SGTextManager2DSFML::~SGTextManager2DSFML()
{

}

void SG::SGTextManager2DSFML::Remove(SGEntity2DHandle* ent)
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

void SG::SGTextManager2DSFML::LoadFont(std::string filepath, SG::SGGuid identifier)
{
	std::unordered_map<SG::SGGuid, sf::Font>::const_iterator got = fonts.find(identifier);

	if (got != fonts.end()) // we already have a font with that identifier, so do nothing
		return;

	sf::Font temp;
	
	if (!temp.loadFromFile(filepath))
	{
		delete &temp; // Force a crash (not a good long term solution)
	}

	fonts[identifier] = temp;
}

void SG::SGTextManager2DSFML::CreateText(SGEntity2DHandle& ent, SG::SGGuid font, std::wstring text, unsigned int size, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha, unsigned int properties)
{
	SGTextSFML temp;

	temp.text.setFont(fonts[font]);
	temp.text.setString(text);
	temp.text.setFillColor(sf::Color(red, green, blue, alpha));
	temp.text.setCharacterSize(size);

	if (properties != 0)
	{
		temp.textProperties = properties;
		int sfmlProp = 0;
		sfmlProp = properties & SG::TextProperties::TEXT_BOLD ? sfmlProp | sf::Text::Bold : sfmlProp;
		sfmlProp = properties & SG::TextProperties::TEXT_ITALIC ? sfmlProp | sf::Text::Italic : sfmlProp;
		sfmlProp = properties & SG::TextProperties::TEXT_UNDERLINED ? sfmlProp | sf::Text::Underlined : sfmlProp;
		if (properties & SG::TextProperties::TEXT_CENTERED)
			temp.text.setOrigin(temp.text.getLocalBounds().left + temp.text.getLocalBounds().width / 2.0f, temp.text.getLocalBounds().top + temp.text.getLocalBounds().height / 2.0f);

		temp.text.setStyle(sfmlProp);
	}	
	
	int index = objects.AddToBack(temp);
	AddComponentToEntity(ent, index);
	
}

void SG::SGTextManager2DSFML::SetOffset(SGEntity2DHandle& ent, float xOffset, float yOffset)
{
	SGEntity2D* temp = nullptr;
	eventHandler->TriggerEvent(SG::BackEndEvent::GET_ENTITY, &temp, &ent);

	if (temp->components[int(componentType)] != -1)
	{
		objects[temp->components[int(componentType)]].xOffset = xOffset;
		objects[temp->components[int(componentType)]].yOffset = yOffset;
	}
}

void SG::SGTextManager2DSFML::SetRotation(SGEntity2DHandle& ent, float rotation)
{
	SGEntity2D* temp = nullptr;
	eventHandler->TriggerEvent(SG::BackEndEvent::GET_ENTITY, &temp, &ent);

	if (temp->components[int(componentType)] != -1)
	{
		objects[temp->components[int(componentType)]].rotation = rotation;
	}
}

void SG::SGTextManager2DSFML::SetText(SGEntity2DHandle& ent, std::wstring text)
{
	SGEntity2D* temp = nullptr;
	eventHandler->TriggerEvent(SG::BackEndEvent::GET_ENTITY, &temp, &ent);

	if (temp->components[int(componentType)] != -1)
	{
		auto& object = objects[temp->components[int(componentType)]];
		object.text.setString(text);

		if(object.textProperties & SG::TextProperties::TEXT_CENTERED)
			object.text.setOrigin(object.text.getLocalBounds().left + object.text.getLocalBounds().width / 2.0f, object.text.getLocalBounds().top + object.text.getLocalBounds().height / 2.0f);
	}
}

void SG::SGTextManager2DSFML::SetFont(SGEntity2DHandle& ent, SG::SGGuid font)
{
	SGEntity2D* temp = nullptr;
	eventHandler->TriggerEvent(SG::BackEndEvent::GET_ENTITY, &temp, &ent);

	if (temp->components[int(componentType)] != -1)
	{
		objects[temp->components[int(componentType)]].text.setFont(fonts[font]);
	}
}

void SG::SGTextManager2DSFML::SetSize(SGEntity2DHandle& ent, unsigned int size)
{
	SGEntity2D* temp = nullptr;
	eventHandler->TriggerEvent(SG::BackEndEvent::GET_ENTITY, &temp, &ent);

	if (temp->components[int(componentType)] != -1)
	{
		objects[temp->components[int(componentType)]].text.setCharacterSize(size);
	}
}

void SG::SGTextManager2DSFML::SetColour(SGEntity2DHandle& ent, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
	SGEntity2D* temp = nullptr;
	eventHandler->TriggerEvent(SG::BackEndEvent::GET_ENTITY, &temp, &ent);

	if (temp->components[int(componentType)] != -1)
	{
		objects[temp->components[int(componentType)]].text.setFillColor(sf::Color(red, green, blue, alpha));
	}
}
