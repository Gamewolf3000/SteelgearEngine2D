#pragma once

#include "SGG2DUtility.h"
#include "SGShapeManager.h"
#include "SGTextureManager2D.h"
#include "SGShaderHandler2D.h"
#include "SGTextManager2D.h"
#include "../SG2DUtility/Headers/SGScene2D.h"

#include <functional>
#include <vector>
#include <mutex>
#include <chrono>

namespace SG
{

	struct SGColour
	{
		uint8_t red;
		uint8_t green;
		uint8_t blue;

		SGColour(uint8_t r, uint8_t g, uint8_t b) : red(r), green(g), blue(b)
		{

		}
	};
	
	struct GraphicsJob
	{
		std::function<void()> function;
		SGGuid identifier;

		GraphicsJob(std::function<void()> func = std::function<void()>(), SGGuid id = SGGuid("")) : function(func), identifier(id)
		{

		}
	};

	class SGCommandBuffer2D
	{
	private:
		friend class SGRenderer2D;

		std::vector<GraphicsJob> jobs;

	public:
		SGCommandBuffer2D(int BufferSize = 0, SGGuid jobIdentifiers[] = nullptr) : jobs(BufferSize)
		{
			for (int i = 0; i < BufferSize; i++)
			{
				jobs[i].identifier = jobIdentifiers[i];
			}
		}

		
		void SetGraphicsJob(GraphicsJob& graphicsJob)
		{
			for (auto& job : jobs)
			{
				if (job.identifier == graphicsJob.identifier)
				{
					job.function = graphicsJob.function;
					return;
				}
			}
		}
	};

	struct SGViewPort2D
	{
		float left;
		float top;
		float width;
		float height;
	};

	struct SGCamera2D
	{
		int xPos;
		int yPos;
		int height;
		int width;
	};

	class SGRenderer2D
	{
	friend class SGEngine2D;

	protected:
		SGGraphicSettings2D settings;
		DebugMessageHandler* debugHandler;

		SGShapeManager* shapeManager;
		SGTextureManager2D* textureManager;
		SGShaderHandler2D* shaderManager;
		SGTextManager2D* textManager;

		SGCommandBuffer2D commandBuffers[2];
		uint8_t inactiveCommandBuffer = 0;
		std::mutex threadMutex;
		bool threadedRendering;
		volatile bool threadIsActive;
		volatile bool hasUpdatedBuffer = false;
		
		virtual void HandleWindowEvents() = 0;
		virtual void RenderThread() = 0;

		const SGVector<SGEntity2D>& GetEntitiesFromScene(const SGSceneEntities& sceneEntities);
		SGGuid GetSceneID(const SGSceneEntities& sceneEntities);
		std::vector<unsigned int> GetSceneOrdering(const SGSceneEntities& sceneEntities);
		void CleanUpManagers();
		void UpdateManagers();
		void ExecuteStoredCommands(int toUse);

		inline void SetRenderPipeline(int index)
		{
			shaderManager->SetRenderPipeline(index);
		}

		inline void WaitForThread()
		{
			if (threadedRendering)
			{
				threadedRendering = false;
			}

			while (threadIsActive)
			{

			}
		}

		inline void SetPrivateBindings(SGEntity2D& ent, SGGuid& sceneID)
		{
			SGEntity2DHandle temp(ent.id, sceneID);
			shaderManager->SetPrivateBindings(temp, ent.components[int(ComponentType::COMPONENT_RENDER_PIPELINE)]);
		}

	public:
		SGRenderer2D(const SGGraphicSettings2D& gSettings, EventManager<BackEndEvent>* eventsBackEnd);
		virtual ~SGRenderer2D();

		virtual void ClearRenderTarget(const SGColour& clearColour) = 0;
		virtual void RenderScene(const SGSceneEntities& sceneEntities, const SGCamera2D& camera, const SGViewPort2D& viewPort) = 0;
		virtual void Display() = 0;
		virtual uint8_t* GetRenderTargetData() = 0;
		virtual void SetRenderTargetData(uint8_t* data) = 0;

		GraphicsJob CreateClearRenderTargetJob(SGGuid identifier, const SGColour& clearColour);
		GraphicsJob CreateRenderSceneJob(SGGuid identifier, SGScene2D& scene, const SGCamera2D& camera, const SGViewPort2D& viewPort);
		GraphicsJob CreateDisplayJob(SGGuid identifier);

		inline SGShapeManager* ShapeManager()
		{
			return shapeManager;
		}

		inline SGTextureManager2D* TextureManager()
		{
			return textureManager;
		}

		inline SGShaderHandler2D* ShaderManager()
		{
			return shaderManager;
		}

		inline SGTextManager2D* TextManager()
		{
			return textManager;
		}

		virtual SGMatrix4x4 GetProjectionMatrix(SGCamera2D& camera) = 0;

		void ExecuteCommandBuffer(const SGCommandBuffer2D& commandBuffer);

		template<typename ClassType, typename... Argtypes, typename... ParameterTypes>
		GraphicsJob CreateGraphicsJob(SGGuid identifier, ClassType* object, void(ClassType::* ptr)(Argtypes...), ParameterTypes&& ...parameters)
		{
			GraphicsJob temp;
			temp.identifier = identifier;
			temp.function = { std::bind(ptr, object, parameters...) };

			return temp;
		}
	};

}