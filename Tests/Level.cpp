#include "Level.h"

SG::Level::Level(SG::SGEngine2D* engineToUse, SG::SGScene2D* sceneToUse)
{
	engine = engineToUse;
	scene = sceneToUse;
	std::string brickTexture = "E:/VS Projects/SteelgearEngine2D/Tests/TestResources/brick_texture.jpg";
	engine->Graphics()->TextureManager()->LoadTexture(brickTexture);

	for (int i = 0; i < 15; i++)
	{
		SGEntity2DHandle temp = scene->GetEntityManager().CreateEntity();
		engine->Graphics()->ShapeManager()->CreateRectangleShape(temp, 72, 72);
		engine->Graphics()->TextureManager()->SetTexture(temp, SGGuid(brickTexture));
		engine->Transform()->SetPos(temp, -640 + 72 * 2 + 72 * i, 200);
		engine->Graphics()->ShaderManager()->SetRenderPipelineOfEntity(temp, SGGuid("TestGamePipeline"));

		levelBlocks.push_back(temp);
	}

	auto enemyEnt = scene->GetEntityManager().CreateEntity();
	std::string enemyTexture = "E:/VS Projects/SteelgearEngine2D/Tests/TestResources/Ghost.png";
	engine->Graphics()->ShapeManager()->CreateRectangleShape(enemyEnt, 50, 50);
	engine->Graphics()->TextureManager()->LoadTexture(enemyTexture);
	engine->Graphics()->TextureManager()->SetTexture(enemyEnt, SGGuid(enemyTexture));
	engine->Graphics()->ShaderManager()->SetRenderPipelineOfEntity(enemyEnt, SGGuid("TestGamePipeline"));
	engine->Transform()->SetPos(enemyEnt, 0, -100);
	auto script = engine->Script()->LoadScript("E:/VS Projects/SteelgearEngine2D/Tests/TestResources/Enemy1Script.as");

	auto tempEnemy = new GameUnit(engine, enemyEnt, script);
	enemies.push_back(tempEnemy);
}

SG::Level::~Level()
{
	for (auto& enemy : enemies)
	{
		delete enemy;
	}
}

void SG::Level::Update(float dt, GameUnit* player)
{
	for (auto& enemy : enemies)
	{
		enemy->Update(dt);
	}

	auto shapManager = engine->Graphics()->ShapeManager();
	auto playerRect = shapManager->GetShapeRect(player->GetEntity());
	auto playerPreviousRect = player->GetPreviousRect();
	SGPoint playerCenter;
	playerCenter.xPos = playerRect.leftMost + playerRect.width / 2.0f;
	playerCenter.yPos = playerRect.topMost + playerRect.height / 2.0f;

	for (auto& block : levelBlocks)
	{
		auto blockRect = shapManager->GetShapeRect(block);

		if (playerRect.Overlaps(blockRect))
		{
			bool xCollision = false;
			bool yCollision = false;

			SGRect horizontalTest = playerRect;
			horizontalTest.topMost = playerPreviousRect.topMost;
			SGRect verticalTest = playerRect;
			verticalTest.leftMost = playerPreviousRect.leftMost;

			if (blockRect.Overlaps(horizontalTest))
				xCollision = true;
			if (blockRect.Overlaps(verticalTest))
				yCollision = true;

			player->ResetPos(xCollision, yCollision);
		}
	}
}