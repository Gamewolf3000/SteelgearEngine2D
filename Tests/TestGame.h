#pragma once

#include "TestBase.h"
#include "Level.h"
#include "GameUnit.h"
#include "Projectile.h"

namespace SG
{

	class TestGame : public TestBase
	{
	private:

		enum GameControls
		{
			MOVE_LEFT,
			MOVE_RIGHT,
			MOVE_UP,
			MOVE_DOWN
		};

		class ScriptInputWrapper
		{
		private:
			SGInputHandler* input;

		public:

			ScriptInputWrapper()
			{

			}

			void SetInputClass(SGInputHandler* inputPtr)
			{
				input = inputPtr;
			}

			bool IsDown(int identifier)
			{
				return input->IsDown(identifier);
			}
		};

		ScriptInputWrapper inputWrapper;

		SGScene2D * scene;
		SG::SGCommandBuffer2D* cmdBuffer;
		SG::SGCamera2D camera;
		SG::SGViewPort2D viewPort;

		Level* level;
		GameUnit* player;
		std::vector<Projectile> projectiles;

		void SetupGraphics();
		void ExposeToScripts();
		void CreatePlayer();

	public:
		TestGame(SG::SGEngine2D* toUse);
		virtual ~TestGame();

		void Update(float dt) override;
		void Render() override;

	};
}