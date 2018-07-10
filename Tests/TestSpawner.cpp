#include "TestSpawner.h"

void SG::TestSpawner::SpawnRandomObject()
{
	float rotation = (rand() % 628) / 100.0f;
	float speed = (rand() % 10000) / 100.0f + 5.0f;
	float lifeTime = (rand() % 700) / 100.0f + 3.0f;

	auto ent = scene->GetEntityManager().CreateEntity();
	engine->Graphics()->ShapeManager()->CreateRectangleShape(ent, 50.0f, 50.0f);
	engine->Graphics()->TextureManager()->SetTexture(ent, std::string("C:/Users/Joakim/Pictures/Brock_Stadium.png"));
	engine->Graphics()->ShaderManager()->SetRenderPipelineOfEntity(ent, SG::SGGuid("TestPipeline"));
	engine->Transform()->Rotate(ent, rotation);

	objects.push_back(SpawnedObject(rotation, speed, lifeTime, ent));
}

SG::TestSpawner::TestSpawner(SG::SGEngine2D * toUse) : TestBase(toUse)
{
	scene = engine->CreateScene(0, 10, SG::SGGuid("testSpawnerScene"));

	SG::SGGuid commandGuids[3] = { SG::SGGuid("clear"), SG::SGGuid("render"), SG::SGGuid("display") };
	cmdBuffer = new SG::SGCommandBuffer2D(3, commandGuids);

	auto clearJob = engine->Graphics()->CreateGraphicsJob(SG::SGGuid("clear"), engine->Graphics(), &SG::SGRenderer2D::ClearRenderTarget, SG::SGColour(0, 0, 255));
	cmdBuffer->SetGraphicsJob(clearJob);
	auto textureString = std::string("C:/Users/Joakim/Pictures/Brock_Stadium.png");
	engine->Graphics()->TextureManager()->LoadTexture(textureString);
	engine->Graphics()->ShaderManager()->UpdateGlobalData(SG::SGGuid("ProjectionMatrix"), engine->Graphics()->GetProjectionMatrix(camera));

	camera.xPos = 0;
	camera.yPos = 0;
	camera.width = 1280;
	camera.height = 720;

	viewPort.top = 0.0f;
	viewPort.left = 0.0f;
	viewPort.width = 1.0f;
	viewPort.height = 1.0f;

	textEnt = scene->GetEntityManager().CreateEntity();
	engine->Graphics()->ShapeManager()->CreateRectangleShape(textEnt, 100.0f, 100.0f);
	engine->Graphics()->TextManager()->LoadFont("C:/Windows/Fonts/consola.ttf", SG::SGGuid("ConsolasFont"));
	engine->Graphics()->TextManager()->CreateText(textEnt, SG::SGGuid("ConsolasFont"), L"Test Text that is actually kinda long but hey that is fine cause you know whatever!\nAlso here is hopefully a new line?", 24, 255, 0, 0, 255, SG::TextProperties::TEXT_CENTERED);

	SG::ShaderBinding shaderBinding[3];
	SG::ShaderDataSourceType shaderSource[3];
	shaderBinding[0].dataType = SG::ShaderDataType::SHADER_TRANSFORM;
	shaderBinding[0].identifier;
	shaderBinding[0].nameOnShader = "world";
	shaderSource[0] = SG::ShaderDataSourceType::HANDLER;
	shaderBinding[1].dataType = SG::ShaderDataType::SHADER_4X4;
	shaderBinding[1].identifier = SG::SGGuid("ProjectionMatrix");
	shaderBinding[1].nameOnShader = "projection";
	shaderSource[1] = SG::ShaderDataSourceType::GLOBAL;
	shaderBinding[2].dataType = SG::ShaderDataType::SHADER_TEXTURE;
	shaderBinding[2].identifier;
	shaderBinding[2].nameOnShader = "texture";
	shaderSource[2] = SG::ShaderDataSourceType::HANDLER;

	engine->Graphics()->ShaderManager()->CreateRenderPipeline(SG::SGGuid("TestPipeline"), shaderBinding, shaderSource, 3, "E:/VS Projects/SteelGearGraphics2D/BlastOff/Shaders/vsTest.txt", "", "E:/VS Projects/SteelGearGraphics2D/BlastOff/Shaders/psTest.txt");

	auto renderJob = engine->Graphics()->CreateGraphicsJob(SG::SGGuid("render"), engine->Graphics(), &SG::SGRenderer2D::RenderScene, scene->GetSceneEntities(), camera, viewPort);
	cmdBuffer->SetGraphicsJob(renderJob);
	auto displayJob = engine->Graphics()->CreateGraphicsJob(SG::SGGuid("display"), engine->Graphics(), &SG::SGRenderer2D::Display);
	cmdBuffer->SetGraphicsJob(displayJob);
}

SG::TestSpawner::~TestSpawner()
{
	delete cmdBuffer;
	engine->DestroyScene(scene);
}

void SG::TestSpawner::Update(float dt)
{
	static int nrOfUpdates = 0;
	timeTilSpawn -= dt;

	engine->Transform()->Rotate(textEnt, 0.05 * dt);

	if (timeTilSpawn <= 0.0f)
	{
		SpawnRandomObject();
		timeTilSpawn = (rand() % 151) / 100.0f + 0.5f;
	}

	for (auto& object : objects)
	{
		object.Update(dt, engine);

		if (object.GetRemainingTime() <= 0.0f)
		{
			scene->GetEntityManager().Remove(&object.GetEntity());
			object = objects.back();
			objects.pop_back();
		}
	}

	auto renderJob = engine->Graphics()->CreateGraphicsJob(SG::SGGuid("render"), engine->Graphics(), &SG::SGRenderer2D::RenderScene, scene->GetSceneEntities(), camera, viewPort);
	cmdBuffer->SetGraphicsJob(renderJob);
}

void SG::TestSpawner::Render()
{
	engine->Graphics()->ExecuteCommandBuffer(*cmdBuffer);
}
