#include "SGTextureManager2D.h"

void SG::SGTextureManager2D::UpdateData()
{
	mutexLock.lock();

	toClean.insert(toClean.end(), toCleanBuffer.begin(), toCleanBuffer.end());
	toCleanBuffer.clear();

	mutexLock.unlock();
}

SG::SGTextureManager2D::SGTextureManager2D()
{
}

SG::SGTextureManager2D::~SGTextureManager2D()
{
}
