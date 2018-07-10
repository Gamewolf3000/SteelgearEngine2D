#pragma once

#include "SGManager.h"
#include "SGTextManager2D.h"
#include <SFML\Graphics.hpp>

#include <vector>
#include <unordered_map>

namespace SG
{
	struct SGTextSFML
	{
		sf::Text text;
		bool active = false;
		SG::SGGuid identifier;
		float xOffset = 0.0f;
		float yOffset = 0.0f;
		float rotation = 0.0f;

		bool operator==(const SGTextSFML& other) const
		{
			return identifier == other.identifier;
		}
	};

	class SGTextManager2DSFML : public SGManager<SGTextSFML>, public SGTextManager2D
	{
	private:
		void CleanUp() override;

		std::unordered_map<SG::SGGuid, sf::Font, SG::SGGuid> fonts;

	public:
		SGTextManager2DSFML(unsigned int startNr, unsigned int expandAmount, EventManager<BackEndEvent>* eventsBackEnd);
		~SGTextManager2DSFML();

		void Remove(SGEntity2DHandle* ent) override;

		void LoadFont(std::string filepath, SG::SGGuid identifier) override;
		void CreateText(SGEntity2DHandle& ent, SG::SGGuid font, std::wstring text = L"", unsigned int size = 10, uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0, uint8_t alpha = 255, unsigned int properties = 0) override;

		void SetOffset(SGEntity2DHandle& ent, float xOffset, float yOffset) override;
		void SetRotation(SGEntity2DHandle& ent, float rotation) override;
		void SetText(SGEntity2DHandle& ent, std::wstring text) override;
		void SetFont(SGEntity2DHandle& ent, SG::SGGuid font) override;
		void SetSize(SGEntity2DHandle& ent, unsigned int size) override;
		void SetColour(SGEntity2DHandle& ent, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) override;

		inline SGTextSFML* GetText(int index)
		{
			return &objects[index];
		}
	};
}