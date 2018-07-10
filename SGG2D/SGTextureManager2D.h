#pragma once

#include "SGG2DUtility.h"
#include <mutex>
#include <string>

namespace SG
{

	class SGTextureManager2D
	{
		friend class SGRenderer2D;

	protected:
		std::vector<int> toClean;
		std::vector<int> toCleanBuffer;
		std::mutex mutexLock;

		virtual void CleanUp() = 0;
		void UpdateData();

	public:
		SGTextureManager2D();
		virtual ~SGTextureManager2D();

		virtual void LoadTexture(std::string& textureFile) = 0;
		virtual void LoadTexture(SGEntity2DHandle& ent, std::string& textureFile) = 0;
		virtual bool SetTexture(SGEntity2DHandle& ent, SGGuid texture) = 0;

	};
}