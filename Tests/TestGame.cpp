#include "TestGame.h"

SG::TestGame::TestGame(SG::SGEngine2D * toUse) : TestBase(toUse)
{
	scene = engine->CreateScene(0, 10, SG::SGGuid("TestGameScene"));

	SetupGraphics();
	ExposeToScripts();
	CreatePlayer();

	engine->Input()->AddSource(GameControls::MOVE_DOWN);
	engine->Input()->AddSource(GameControls::MOVE_UP);
	engine->Input()->AddSource(GameControls::MOVE_LEFT);
	engine->Input()->AddSource(GameControls::MOVE_RIGHT);

	engine->Input()->BindKey(GameControls::MOVE_DOWN, SG::KeyboardInput::KEY_DOWN);
	engine->Input()->BindKey(GameControls::MOVE_UP, SG::KeyboardInput::KEY_UP);
	engine->Input()->BindKey(GameControls::MOVE_LEFT, SG::KeyboardInput::KEY_LEFT);
	engine->Input()->BindKey(GameControls::MOVE_RIGHT, SG::KeyboardInput::KEY_RIGHT);

	level = new Level(engine, scene);
}

void SG::TestGame::SetupGraphics()
{
	SG::SGGuid commandGuids[3] = { SG::SGGuid("clear"), SG::SGGuid("render"), SG::SGGuid("display") };
	cmdBuffer = new SG::SGCommandBuffer2D(3, commandGuids);

	auto clearJob = engine->Graphics()->CreateGraphicsJob(SG::SGGuid("clear"), engine->Graphics(), &SG::SGRenderer2D::ClearRenderTarget, SG::SGColour(0, 0, 255));
	cmdBuffer->SetGraphicsJob(clearJob);
	engine->Graphics()->ShaderManager()->UpdateGlobalData(SG::SGGuid("ProjectionMatrix"), engine->Graphics()->GetProjectionMatrix(camera));

	camera.xPos = 0;
	camera.yPos = 0;
	camera.width = 1280;
	camera.height = 720;

	viewPort.top = 0.0f;
	viewPort.left = 0.0f;
	viewPort.width = 1.0f;
	viewPort.height = 1.0f;

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

	engine->Graphics()->ShaderManager()->CreateRenderPipeline(SG::SGGuid("TestGamePipeline"), shaderBinding, shaderSource, 3, "E:/VS Projects/SteelGearGraphics2D/BlastOff/Shaders/vsTest.txt", "", "E:/VS Projects/SteelGearGraphics2D/BlastOff/Shaders/psTest.txt");

	auto renderJob = engine->Graphics()->CreateGraphicsJob(SG::SGGuid("render"), engine->Graphics(), &SG::SGRenderer2D::RenderScene, scene->GetSceneEntities(), camera, viewPort);
	cmdBuffer->SetGraphicsJob(renderJob);
	auto displayJob = engine->Graphics()->CreateGraphicsJob(SG::SGGuid("display"), engine->Graphics(), &SG::SGRenderer2D::Display);
	cmdBuffer->SetGraphicsJob(displayJob);
}

void SG::TestGame::ExposeToScripts()
{
	auto s = engine->Script();
	auto a = &SGPoint::operator=;
	s->ExposeClass<SGPoint>("SGPoint");
	s->ExposeObjectProperty("SGPoint", "float xPos", offsetof(SGPoint, xPos));
	s->ExposeObjectProperty("SGPoint", "float yPos", offsetof(SGPoint, yPos));
	s->ExposeObjectMethod(&SGPoint::operator=, "SGPoint", "SGPoint& opAssign(SGPoint &in)");

	s->ExposeClass<GameUnit>("GameUnit");
	s->ExposeObjectMethod(&GameUnit::GetHealth, "GameUnit", "int GetHealth()");
	s->ExposeObjectMethod(&GameUnit::Move, "GameUnit", "void Move(float xToMove, float yToMove)");
	s->ExposeObjectMethod(&GameUnit::GetPosition, "GameUnit", "SGPoint GetPosition()");

	inputWrapper.SetInputClass(engine->Input());
	s->ExposeClass<TestGame::ScriptInputWrapper>("ScriptInputWrapper");
	s->ExposeObject("ScriptInputWrapper input", &inputWrapper);
	s->ExposeObjectMethod(&ScriptInputWrapper::IsDown, "ScriptInputWrapper", "bool IsDown(int)");


}

void SG::TestGame::CreatePlayer()
{
	auto playerEnt = scene->GetEntityManager().CreateEntity();
	std::string playerTexture = "E:/VS Projects/SteelgearEngine2D/Tests/TestResources/tinman.png";
	engine->Graphics()->ShapeManager()->CreateRectangleShape(playerEnt, 50, 50);
	engine->Graphics()->TextureManager()->LoadTexture(playerTexture);
	engine->Graphics()->TextureManager()->SetTexture(playerEnt, SGGuid(playerTexture));
	engine->Graphics()->ShaderManager()->SetRenderPipelineOfEntity(playerEnt, SGGuid("TestGamePipeline"));
	auto script = engine->Script()->LoadScript("E:/VS Projects/SteelgearEngine2D/Tests/TestResources/PlayerScript.as");

	player = new GameUnit(engine, playerEnt, script);
	engine->Script()->ExposeObject("GameUnit player", player);
}

SG::TestGame::~TestGame()
{
	delete level;
	delete player;
	delete cmdBuffer;
	engine->DestroyScene(scene);
}

void SG::TestGame::Update(float dt)
{
	player->Update(dt);
	level->Update(dt, player);

	auto renderJob = engine->Graphics()->CreateGraphicsJob(SG::SGGuid("render"), engine->Graphics(), &SG::SGRenderer2D::RenderScene, scene->GetSceneEntities(), camera, viewPort);
	cmdBuffer->SetGraphicsJob(renderJob);
}

void SG::TestGame::Render()
{
	engine->Graphics()->ExecuteCommandBuffer(*cmdBuffer);
}
