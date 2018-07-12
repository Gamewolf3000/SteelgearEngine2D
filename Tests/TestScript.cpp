#include "TestScript.h"
#include <locale>
#include <codecvt>
#include <string>

SG::TestScript::TestScript(SG::SGEngine2D * toUse) : TestBase(toUse)
{
	scene = engine->CreateScene(0, 10, SG::SGGuid("testScriptScene"));

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

	engine->Graphics()->ShaderManager()->CreateRenderPipeline(SG::SGGuid("TestPipeline2"), shaderBinding, shaderSource, 3, "E:/VS Projects/SteelGearGraphics2D/BlastOff/Shaders/vsTest.txt", "", "E:/VS Projects/SteelGearGraphics2D/BlastOff/Shaders/psTest.txt");

	auto renderJob = engine->Graphics()->CreateGraphicsJob(SG::SGGuid("render"), engine->Graphics(), &SG::SGRenderer2D::RenderScene, scene->GetSceneEntities(), camera, viewPort);
	cmdBuffer->SetGraphicsJob(renderJob);
	auto displayJob = engine->Graphics()->CreateGraphicsJob(SG::SGGuid("display"), engine->Graphics(), &SG::SGRenderer2D::Display);
	cmdBuffer->SetGraphicsJob(displayJob);

	std::string scriptString;
	engine->Script()->ExposeObject<std::string>("string toSet", &scriptString);
	script = engine->Script()->LoadScript("TestScript.as");
	engine->Script()->ExecuteScript(script, "void GetText()");

	std::wstring wide(scriptString.length(), L' '); // Make room for characters
	std::copy(scriptString.begin(), scriptString.end(), wide.begin()); // Copy string to wstring.

	engine->Graphics()->TextManager()->SetText(textEnt, wide);
}

SG::TestScript::~TestScript()
{
	delete cmdBuffer;
	engine->DestroyScene(scene);
}

void SG::TestScript::Update(float dt)
{
	engine->Transform()->Rotate(textEnt, 0.05 * dt);

	auto renderJob = engine->Graphics()->CreateGraphicsJob(SG::SGGuid("render"), engine->Graphics(), &SG::SGRenderer2D::RenderScene, scene->GetSceneEntities(), camera, viewPort);
	cmdBuffer->SetGraphicsJob(renderJob);
}

void SG::TestScript::Render()
{
	engine->Graphics()->ExecuteCommandBuffer(*cmdBuffer);
}
