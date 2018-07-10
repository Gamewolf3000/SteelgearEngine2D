#pragma once

#include "SGManager.h"
#include "SGShaderHandler2D.h"
#include "SGTextureManager2DSFML.h"
#include "SGShapeManagerSFML.h"
#include <SFML\Graphics.hpp>

#include <vector>

namespace SG
{
	struct SGRenderPipelineSFML
	{
		std::vector<ShaderBinding> globalBindings;
		std::vector<ShaderBinding> privateBindings;
		std::vector<ShaderBinding> handlerBindings;
		sf::Shader* shaders;

		SGGuid id;

		bool operator==(const SGRenderPipelineSFML& other) const
		{
			return this->id == other.id;
		}
	};

	class SGShaderHandler2DSFML : public SGManager<SGRenderPipelineSFML>, public SGShaderHandler2D
	{
	private:
		void CleanUp() override;

		void SetRenderPipeline(unsigned int pipelineId) override;
		void SetPrivateBindings(SGEntity2DHandle& ent, unsigned int pipelineId) override;

	public:
		SGShaderHandler2DSFML(unsigned int startNr, unsigned int expandAmount, EventManager<BackEndEvent>* eventsBackEnd);
		~SGShaderHandler2DSFML();

		void CreateRenderPipeline(SGGuid identifier, ShaderBinding* shaderBindings, ShaderDataSourceType* bindingSources, unsigned int nrOfBindings, std::string vsFileName, std::string gsFileName, std::string psFileName) override;

		bool SetRenderPipelineOfEntity(SGEntity2DHandle& ent, SGGuid identifier) override;
		void Remove(SGEntity2DHandle* ent) override;

		void SetHandlerBindings(SGEntity2D& ent, SGTextureManager2DSFML* textureManager, SGShapeManagerSFML* shapeManager, unsigned int pipelineId);

		inline sf::Shader* GetShader(int index)
		{
			return objects[index].shaders;
		}
	};
}