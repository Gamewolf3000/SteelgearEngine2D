#pragma once

#include "SGG2DUtility.h"
#include <variant>
#include <unordered_map>
#include <mutex>
#include <string>

namespace SG
{

	typedef std::variant<float, int, bool, SGMatrix3x3, SGMatrix4x4, SGVec3, SGVec4> ShaderData;

	enum class ShaderDataSourceType
	{
		HANDLER,
		PRIVATE,
		GLOBAL
	};

	enum class ShaderDataType
	{
		SHADER_FLOAT,
		SHADER_INT,
		SHADER_BOOL,
		SHADER_VEC3,
		SHADER_VEC4,
		SHADER_4X4,
		SHADER_3X3,
		SHADER_TEXTURE,
		SHADER_TRANSFORM

	};

	struct ShaderBinding
	{
		std::string nameOnShader;
		ShaderDataType dataType;
		SGGuid identifier;
	};

	class SGShaderHandler2D
	{
		friend class SGRenderer2D;

	protected:
		std::vector<int> toClean;
		std::vector<int> toCleanBuffer;
		std::mutex mutexLock;

		std::unordered_map<SGGuid, ShaderData, SGGuid> globalData;
		std::unordered_map<SGGuid, std::unordered_map<SGEntity2DHandle, ShaderData, SGEntity2DHandle>, SGGuid> privateData;

		std::unordered_map<SGGuid, ShaderData, SGGuid> globalDataToUpdate;
		std::unordered_map<SGGuid, std::unordered_map<SGEntity2DHandle, ShaderData, SGEntity2DHandle>, SGGuid> privateDataToUpdate;

		std::unordered_map<SGGuid, ShaderData, SGGuid> globalDataBuffer;
		std::unordered_map<SGGuid, std::unordered_map<SGEntity2DHandle, ShaderData, SGEntity2DHandle>, SGGuid> privateDataBuffer;

		virtual void CleanUp() = 0;
		void UpdateData();

		virtual void SetRenderPipeline(unsigned int pipelineId) = 0;
		virtual void SetPrivateBindings(SGEntity2DHandle& ent, unsigned int pipelineId) = 0;

	public:
		SGShaderHandler2D();
		virtual ~SGShaderHandler2D();

		virtual void CreateRenderPipeline(SGGuid identifier, ShaderBinding* shaderBindings, ShaderDataSourceType* bindingSources, unsigned int nrOfBindings, std::string vsFileName, std::string gsFileName, std::string psFileName) = 0;
		virtual bool SetRenderPipelineOfEntity(SGEntity2DHandle& ent, SGGuid identifier) = 0;

		void UpdateGlobalData(SGGuid identifier, ShaderData data);
		void UpdatePrivateData(SGGuid identifier, SGEntity2DHandle& ent, ShaderData data);


	};
}