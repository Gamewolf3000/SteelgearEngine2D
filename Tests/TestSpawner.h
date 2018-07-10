#pragma once

#include "TestBase.h"
#include <math.h>
#include <random>

namespace SG
{

	class TestSpawner : public TestBase
	{
	private:

		class SpawnedObject
		{
		private:
			float rotation;
			float speed;
			float lifeTime;
			SG::SGEntity2DHandle entity;

		public:
			SpawnedObject(float rot, float spd, float time, SG::SGEntity2DHandle ent)
			{
				rotation = rot;
				speed = spd;
				lifeTime = time;
				entity = ent;
				
			}

			inline void Update(float dt, SG::SGEngine2D* toUse)
			{
				lifeTime -= dt;
				
				toUse->Transform()->Move(entity, speed * dt * sin(rotation), speed * dt * cos(rotation));
			}

			inline float GetRemainingTime()
			{
				return lifeTime;
			}

			inline SG::SGEntity2DHandle& GetEntity()
			{
				return entity;
			}

		};

		SGScene2D * scene;
		SG::SGCommandBuffer2D* cmdBuffer;
		std::vector<SpawnedObject> objects;
		float timeTilSpawn = 0.0f;
		SG::SGEntity2DHandle textEnt;

		SG::SGCamera2D camera;
		SG::SGViewPort2D viewPort;

		void SpawnRandomObject();

	public:
		TestSpawner(SG::SGEngine2D* toUse);
		virtual ~TestSpawner();

		void Update(float dt) override;
		void Render() override;

	};
}