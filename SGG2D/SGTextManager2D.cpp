#include "SGTextManager2D.h"

void SG::SGTextManager2D::UpdateData()
{
	mutexLock.lock();

	toClean.insert(toClean.end(), toCleanBuffer.begin(), toCleanBuffer.end());
	toCleanBuffer.clear();

	mutexLock.unlock();
}

SG::SGTextManager2D::SGTextManager2D()
{

}

SG::SGTextManager2D::~SGTextManager2D()
{

}