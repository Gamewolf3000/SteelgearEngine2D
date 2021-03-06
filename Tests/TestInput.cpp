#include "TestInput.h"
#include <locale>
#include <codecvt>
#include <string>

SG::TestInput::TestInput(SG::SGEngine2D * toUse) : TestBase(toUse)
{
	scene = engine->CreateScene(0, 10, SG::SGGuid("testInputScene"));

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

	engine->Graphics()->ShaderManager()->CreateRenderPipeline(SG::SGGuid("TestPipeline3"), shaderBinding, shaderSource, 3, "E:/VS Projects/SteelGearGraphics2D/BlastOff/Shaders/vsTest.txt", "", "E:/VS Projects/SteelGearGraphics2D/BlastOff/Shaders/psTest.txt");

	auto renderJob = engine->Graphics()->CreateGraphicsJob(SG::SGGuid("render"), engine->Graphics(), &SG::SGRenderer2D::RenderScene, scene->GetSceneEntities(), camera, viewPort);
	cmdBuffer->SetGraphicsJob(renderJob);
	auto displayJob = engine->Graphics()->CreateGraphicsJob(SG::SGGuid("display"), engine->Graphics(), &SG::SGRenderer2D::Display);
	cmdBuffer->SetGraphicsJob(displayJob);

	engine->Input()->AddSource(0);
	engine->Input()->BindKey(0, SG::KeyboardInput::KEY_LEFT);
	engine->Input()->AddSource(1);
	engine->Input()->BindKey(1, SG::KeyboardInput::KEY_RIGHT);

	engine->Input()->AddSource(0);
	engine->Input()->BindButton(0, SG::ControllerInput::BUTTON_1, 0);
	engine->Input()->AddSource(1);
	engine->Input()->BindButton(1, SG::ControllerInput::BUTTON_2, 0);

	ent = scene->GetEntityManager().CreateEntity();
	textEnt = scene->GetEntityManager().CreateEntity();
	engine->Graphics()->ShapeManager()->CreateRectangleShape(ent, 20, 20);
	engine->Graphics()->TextManager()->LoadFont("C:/Windows/Fonts/consola.ttf", SG::SGGuid("ConsolasFont"));
	engine->Graphics()->TextManager()->CreateText(textEnt, SG::SGGuid("ConsolasFont"), L"", 30);
	engine->Transform()->SetPos(textEnt, -640.0f, -360.0f);
}

SG::TestInput::~TestInput()
{
	delete cmdBuffer;
	engine->DestroyScene(scene);
}

void SG::TestInput::Update(float dt)
{
	static float totalMovement = 0;

	if (engine->Input()->IsDown(0) || engine->Input()->GetControllerAxisValue(0, SG::ControllerAxis::AXIS_1) > 0.0f)
		totalMovement -= 1000.0f * dt;

	if (engine->Input()->IsDown(1) || engine->Input()->GetControllerAxisValue(0, SG::ControllerAxis::AXIS_1) < 0.0f)
		totalMovement += 1000.0f * dt;

	camera.xPos = totalMovement;

	auto pixelCoords = engine->Input()->GetMousePos();
	auto worldCoords = engine->Graphics()->PixelToPosition(pixelCoords);
	engine->Graphics()->TextManager()->SetText(textEnt, std::wstring(L"Mouse pos in pixels: (X = ") + std::to_wstring(pixelCoords.xPos) + std::wstring(L" | Y = ") + std::to_wstring(pixelCoords.yPos) + std::wstring(L")\nMouse pos translated to world pos: (X = ") + std::to_wstring(worldCoords.xPos) + std::wstring(L" | Y = ") + std::to_wstring(worldCoords.yPos) + std::wstring(L")"));

	auto renderJob = engine->Graphics()->CreateGraphicsJob(SG::SGGuid("render"), engine->Graphics(), &SG::SGRenderer2D::RenderScene, scene->GetSceneEntities(), camera, viewPort);
	cmdBuffer->SetGraphicsJob(renderJob);
}

void SG::TestInput::Render()
{
	engine->Graphics()->ExecuteCommandBuffer(*cmdBuffer);
}
