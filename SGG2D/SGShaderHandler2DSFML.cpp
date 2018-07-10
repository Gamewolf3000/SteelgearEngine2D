#include "SGShaderHandler2DSFML.h"

void SG::SGShaderHandler2DSFML::CleanUp()
{
	mutexLock.lock();

	for (auto& index : toClean)
	{
		if (objects.GetReferenceCountOfElement(index) == 1)
		{
			delete objects[index].shaders;
			objects[index].shaders = nullptr;
		}

		objects.Remove(index);
	}

	globalData = globalDataToUpdate;
	privateData = privateDataToUpdate;

	toClean.clear();

	mutexLock.unlock();
}

void SG::SGShaderHandler2DSFML::SetRenderPipeline(unsigned int pipelineId)
{
	SGRenderPipelineSFML& temp = objects[pipelineId];

	for (auto& binding : temp.globalBindings)
	{
		auto& data = globalData[binding.identifier];

		if (binding.dataType == SG::ShaderDataType::SHADER_INT)
		{
			temp.shaders->setUniform(binding.nameOnShader, std::get<int>(data));
		}
		else if (binding.dataType == SG::ShaderDataType::SHADER_FLOAT)
		{
			temp.shaders->setUniform(binding.nameOnShader, std::get<float>(data));
		}
		else if (binding.dataType == SG::ShaderDataType::SHADER_BOOL)
		{
			temp.shaders->setUniform(binding.nameOnShader, std::get<bool>(data));
		}
		else if (binding.dataType == SG::ShaderDataType::SHADER_3X3)
		{
			temp.shaders->setUniform(binding.nameOnShader, sf::Glsl::Mat3(std::get<SGMatrix3x3>(data).elements));
		}
		else if (binding.dataType == SG::ShaderDataType::SHADER_4X4)
		{
			temp.shaders->setUniform(binding.nameOnShader, sf::Glsl::Mat4(std::get<SGMatrix4x4>(data).elements));
		}
	}
}

void SG::SGShaderHandler2DSFML::SetPrivateBindings(SGEntity2DHandle& ent, unsigned int pipelineId)
{
	SGRenderPipelineSFML& temp = objects[pipelineId];
	
	for (auto& binding : temp.privateBindings)
	{
		auto& data = privateData[binding.identifier][ent];

		if (binding.dataType == SG::ShaderDataType::SHADER_INT)
		{
			temp.shaders->setUniform(binding.nameOnShader, std::get<int>(data));
		}
		else if (binding.dataType == SG::ShaderDataType::SHADER_FLOAT)
		{
			temp.shaders->setUniform(binding.nameOnShader, std::get<float>(data));
		}
		else if (binding.dataType == SG::ShaderDataType::SHADER_BOOL)
		{
			temp.shaders->setUniform(binding.nameOnShader, std::get<bool>(data));
		}
		else if (binding.dataType == SG::ShaderDataType::SHADER_3X3)
		{
			temp.shaders->setUniform(binding.nameOnShader, sf::Glsl::Mat3(std::get<SGMatrix3x3>(data).elements));
		}
		else if (binding.dataType == SG::ShaderDataType::SHADER_4X4)
		{
			temp.shaders->setUniform(binding.nameOnShader, sf::Glsl::Mat4(std::get<SGMatrix4x4>(data).elements));
		}
		else if (binding.dataType == SG::ShaderDataType::SHADER_VEC3)
		{
			auto value = std::get<SGVec3>(data).elements;
			temp.shaders->setUniform(binding.nameOnShader, sf::Glsl::Vec3(value[0], value[1], value[2]));
		}
		else if (binding.dataType == SG::ShaderDataType::SHADER_VEC4)
		{
			auto value = std::get<SGVec4>(data).elements;
			temp.shaders->setUniform(binding.nameOnShader, sf::Glsl::Vec4(value[0], value[1], value[2], value[3]));
		}
	}
}

SG::SGShaderHandler2DSFML::SGShaderHandler2DSFML(unsigned int startNr, unsigned int expandAmount, EventManager<BackEndEvent>* eventsBackEnd) : SGManager(startNr, expandAmount, eventsBackEnd, ComponentType::COMPONENT_RENDER_PIPELINE)
{

}

SG::SGShaderHandler2DSFML::~SGShaderHandler2DSFML()
{
	for (int i = 0; i < objects.Size(); i++)
	{
		delete objects[i].shaders;
	}
}

void SG::SGShaderHandler2DSFML::CreateRenderPipeline(SGGuid identifier, ShaderBinding* shaderBindings, ShaderDataSourceType* bindingSources, unsigned int nrOfBindings, std::string vsFileName, std::string gsFileName, std::string psFileName)
{
	SGRenderPipelineSFML temp;

	temp.id = identifier;

	if (objects.Find(temp) != -1) // If there is already such a pipeline then we just return
		return;

	temp.shaders = new sf::Shader();
	bool load = false;

	if (vsFileName != "")
		load = temp.shaders->loadFromFile(vsFileName, sf::Shader::Type::Vertex);

	if (gsFileName != "")
		load = temp.shaders->loadFromFile(gsFileName, sf::Shader::Type::Geometry);

	if(psFileName != "")
		load = temp.shaders->loadFromFile(psFileName, sf::Shader::Type::Fragment);

	for (int i = 0; i < nrOfBindings; i++)
	{
		if (bindingSources[i] == ShaderDataSourceType::GLOBAL)
		{
			temp.globalBindings.push_back(shaderBindings[i]);
		}
		else if (bindingSources[i] == ShaderDataSourceType::PRIVATE)
		{
			temp.privateBindings.push_back(shaderBindings[i]);
		}
		else if (bindingSources[i] == ShaderDataSourceType::HANDLER)
		{
			temp.handlerBindings.push_back(shaderBindings[i]);
		}
	}

	objects.Add(temp);
}

bool SG::SGShaderHandler2DSFML::SetRenderPipelineOfEntity(SGEntity2DHandle & ent, SGGuid identifier)
{
	SGRenderPipelineSFML temp;
	temp.id = identifier;
	int index = objects.Find(temp);

	if (index != -1)
	{
		AddComponentToEntity(ent, index);
		eventHandler->TriggerEvent(SG::BackEndEvent::SET_DIRTY_SCENE, &ent);
		return true;
	}

	return false;
}

void SG::SGShaderHandler2DSFML::Remove(SGEntity2DHandle* ent)
{
	SGEntity2D* temp = nullptr;
	eventHandler->TriggerEvent(SG::BackEndEvent::GET_ENTITY, &temp, ent);

	if (temp->components[int(componentType)] != -1)
	{

		mutexLock.lock();
		toCleanBuffer.push_back(temp->components[int(componentType)]);
		mutexLock.unlock();

		this->RemoveComponentFromEntity(temp);
		eventHandler->TriggerEvent(SG::BackEndEvent::SET_DIRTY_SCENE, ent);

	}
}

void SG::SGShaderHandler2DSFML::SetHandlerBindings(SGEntity2D& ent, SG::SGTextureManager2DSFML* textureManager, SG::SGShapeManagerSFML* shapeManager, unsigned int pipelineId)
{
	SGRenderPipelineSFML temp = objects[pipelineId];

	for (auto& binding : temp.handlerBindings)
	{

		if (binding.dataType == SG::ShaderDataType::SHADER_TEXTURE)
		{
			temp.shaders->setUniform(binding.nameOnShader, *textureManager->GetTexture(ent.components[int(ComponentType::COMPONENT_TEXTURE)]));
		}
		else if (binding.dataType == SG::ShaderDataType::SHADER_TRANSFORM)
		{
			temp.shaders->setUniform(binding.nameOnShader, sf::Glsl::Mat4(shapeManager->GetShape(ent.components[int(ComponentType::COMPONENT_SHAPE)])->getTransform().getMatrix()));
		}
	}
}