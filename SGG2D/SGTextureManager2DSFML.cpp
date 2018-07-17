#include "SGTextureManager2DSFML.h"

void SG::SGTextureManager2DSFML::CleanUp()
{
	mutexLock.lock();

	// COMMENTED OUT FOR NOW!!! It works but if the last reference to a texture is removed then it will be deloaded and thus needed to be loaded later if someone wants it, this should
	//be changed so that it is only removed if some type of max limit is reached, and support should be added to reload old textures, by storing the path perhaps and such?

	//for (auto& index : toClean)
	//{
	//	if (objects.GetReferenceCountOfElement(index) == 1)
	//	{
	//		delete objects[index].textureData;
	//		objects[index].textureData = nullptr;
	//	}
	//	
	//	objects.Remove(index);
	//}

	toClean.clear();

	mutexLock.unlock();
}

SG::SGTextureManager2DSFML::SGTextureManager2DSFML(unsigned int startNr, unsigned int expandAmount, EventManager<BackEndEvent>* eventsBackEnd) : SGManager(startNr, expandAmount, eventsBackEnd, ComponentType::COMPONENT_TEXTURE)
{

}

SG::SGTextureManager2DSFML::~SGTextureManager2DSFML()
{
	for (int i = 0; i < objects.Size(); i++)
	{
		delete objects[i].textureData;
	}
}

void SG::SGTextureManager2DSFML::LoadTexture(std::string& textureFile)
{
	StoredTexture temp;
	temp.textureID = SG::SGGuid(textureFile);
	int index = objects.Find(temp);

	if (index == -1)
	{
		temp.textureData = new sf::Texture();
		temp.textureData->loadFromFile(textureFile);
		objects.AddToBack(temp);
	}

}

void SG::SGTextureManager2DSFML::LoadTexture(SGEntity2DHandle & ent, std::string& textureFile)
{
	StoredTexture temp;
	temp.textureID = SG::SGGuid(textureFile);
	int index = objects.Find(temp);

	if (index != -1)
	{
		AddComponentToEntity(ent, index);
	}
	else
	{
		temp.textureData = new sf::Texture();
		temp.textureData->loadFromFile(textureFile);
		index = objects.AddToBack(temp);
		AddComponentToEntity(ent, index);
	}

	eventHandler->TriggerEvent(SG::BackEndEvent::SET_DIRTY_SCENE, &ent);

}

bool SG::SGTextureManager2DSFML::SetTexture(SGEntity2DHandle & ent, SGGuid texture)
{
	StoredTexture temp;
	temp.textureID = texture;
	int index = objects.Find(temp);

	if (index != -1)
	{
		AddComponentToEntity(ent, index);
		eventHandler->TriggerEvent(SG::BackEndEvent::SET_DIRTY_SCENE, &ent);
		return true;
	}

	return false;
}

void SG::SGTextureManager2DSFML::Remove(SGEntity2DHandle* ent)
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
