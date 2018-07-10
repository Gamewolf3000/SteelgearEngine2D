#include "SGShaderHandler2D.h"

void SG::SGShaderHandler2D::UpdateData()
{
	mutexLock.lock();

	toClean.insert(toClean.end(), toCleanBuffer.begin(), toCleanBuffer.end());
	toCleanBuffer.clear();

	globalDataToUpdate = globalDataBuffer;
	privateDataToUpdate = privateDataBuffer;

	mutexLock.unlock();
}

SG::SGShaderHandler2D::SGShaderHandler2D()
{

}

SG::SGShaderHandler2D::~SGShaderHandler2D()
{

}

void SG::SGShaderHandler2D::UpdateGlobalData(SGGuid identifier, ShaderData data)
{
	globalDataBuffer[identifier] = data;
}

void SG::SGShaderHandler2D::UpdatePrivateData(SGGuid identifier, SGEntity2DHandle & ent, ShaderData data)
{
	privateDataBuffer[identifier][ent] = data;
}
