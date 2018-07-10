#pragma once

#include "SGManager.h"
#include "SGTextureManager2D.h"
#include <SFML\Graphics.hpp>

namespace SG
{
	struct StoredTexture
	{
		SGGuid textureID;
		sf::Texture* textureData;

		bool operator==(const StoredTexture& other) const
		{
			return this->textureID == other.textureID;
		}
	};

	class SGTextureManager2DSFML : public SGManager<StoredTexture>, public SGTextureManager2D
	{
	private:
		void CleanUp() override;

	public:
		SGTextureManager2DSFML(unsigned int startNr, unsigned int expandAmount, EventManager<BackEndEvent>* eventsBackEnd);
		~SGTextureManager2DSFML();

		void LoadTexture(std::string& textureFile) override;
		void LoadTexture(SGEntity2DHandle& ent, std::string& textureFile) override;
		bool SetTexture(SGEntity2DHandle& ent, SGGuid texture) override;

		void Remove(SGEntity2DHandle* ent) override;

		inline sf::Texture* GetTexture(int index)
		{
			return objects[index].textureData;
		}
	};
}