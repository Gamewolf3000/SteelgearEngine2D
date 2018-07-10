#pragma once

#include "SGG2DUtility.h"
#include <mutex>
#include <string>

namespace SG
{

	enum TextProperties
	{
		TEXT_BOLD = 1,
		TEXT_UNDERLINED = 2,
		TEXT_ITALIC = 4,
		TEXT_CENTERED = 8
	};

	class SGTextManager2D
	{
		friend class SGRenderer2D;

	protected:
		std::vector<int> toClean;
		std::vector<int> toCleanBuffer;
		std::mutex mutexLock;

		virtual void CleanUp() = 0;
		void UpdateData();

	public:
		SGTextManager2D();
		virtual ~SGTextManager2D();

		virtual void LoadFont(std::string filepath, SG::SGGuid identifier) = 0;
		virtual void CreateText(SGEntity2DHandle& ent, SG::SGGuid font, std::wstring text = L"", unsigned int size = 10, uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0, uint8_t alpha = 255, unsigned int properties = 0) = 0;

		virtual void SetOffset(SGEntity2DHandle& ent, float xOffset, float yOffset) = 0;
		virtual void SetRotation(SGEntity2DHandle& ent, float rotation) = 0;
		virtual void SetText(SGEntity2DHandle& ent, std::wstring text) = 0;
		virtual void SetFont(SGEntity2DHandle& ent, SG::SGGuid font) = 0;
		virtual void SetSize(SGEntity2DHandle& ent, unsigned int size) = 0;
		virtual void SetColour(SGEntity2DHandle& ent, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) = 0;
		

	};
}