#include "SGShapeManager.h"

void SG::SGShapeManager::UpdateData()
{
	mutexLock.lock();

	toClean.insert(toClean.end(), toCleanBuffer.begin(), toCleanBuffer.end());
	toCleanBuffer.clear();

	mutexLock.unlock();
}

SG::SGShapeManager::SGShapeManager()
{
}

SG::SGShapeManager::~SGShapeManager()
{
}
