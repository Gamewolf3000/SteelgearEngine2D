#include "TestGuiElement.h"
#include <locale>
#include <codecvt>
#include <string>

SG::TestGuiElement::TestGuiElement(SG::SGEngine2D * toUse) : TestBase(toUse)
{
	scene = engine->CreateScene(0, 10, SG::SGGuid("testGuiScene"));

	SG::SGGuid commandGuids[3] = { SG::SGGuid("clear"), SG::SGGuid("render"), SG::SGGuid("display") };
	cmdBuffer = new SG::SGCommandBuffer2D(3, commandGuids);

	auto clearJob = engine->Graphics()->CreateGraphicsJob(SG::SGGuid("clear"), engine->Graphics(), &SG::SGRenderer2D::ClearRenderTarget, SG::SGColour(0, 0, 255));
	cmdBuffer->SetGraphicsJob(clearJob);
	auto textureString = std::string("C:/Users/Joakim/Pictures/Brock_Stadium.png");
	auto textureString2 = std::string("C:/Users/Joakim/Pictures/rival.png");
	engine->Graphics()->TextureManager()->LoadTexture(textureString);
	engine->Graphics()->TextureManager()->LoadTexture(textureString2);
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

	SG::SGGuiSettings guiSettings;
	guiSettings.pipeline = SGGuid("TestPipeline3");
	guiSettings.scriptPath;
	guiSettings.selectedFunctionName;
	guiSettings.text;
	guiSettings.textureSelected = SGGuid("C:/Users/Joakim/Pictures/rival.png");;
	guiSettings.textureNotSelected = SGGuid("C:/Users/Joakim/Pictures/Brock_Stadium.png");
	guiSettings.updateFunctionName;
	guiSettings.xPos = 100;
	guiSettings.yPos = 100;
	guiSettings.xSize = 100;
	guiSettings.ySize = 100;
	guiSettings.checkForInput = true;

	button = engine->CreateGuiElement(scene, guiSettings);

	auto renderJob = engine->Graphics()->CreateGraphicsJob(SG::SGGuid("render"), engine->Graphics(), &SG::SGRenderer2D::RenderScene, scene->GetSceneEntities(), camera, viewPort);
	cmdBuffer->SetGraphicsJob(renderJob);
	auto displayJob = engine->Graphics()->CreateGraphicsJob(SG::SGGuid("display"), engine->Graphics(), &SG::SGRenderer2D::Display);
	cmdBuffer->SetGraphicsJob(displayJob);
}

SG::TestGuiElement::~TestGuiElement()
{
	delete cmdBuffer;
	engine->DestroyScene(scene);
}

void SG::TestGuiElement::Update(float dt)
{
	button.Update();

	//if (engine->Graphics()->ShapeManager()->GetShapeRect(button.GetEntity()).Overlaps(engine->Input()->GetMousePos()))
	//{
	//	button.SetSelected(true);
	//}
	//else
	//{
	//	button.SetSelected(false);
	//}

	auto renderJob = engine->Graphics()->CreateGraphicsJob(SG::SGGuid("render"), engine->Graphics(), &SG::SGRenderer2D::RenderScene, scene->GetSceneEntities(), camera, viewPort);
	cmdBuffer->SetGraphicsJob(renderJob);
}

void SG::TestGuiElement::Render()
{
	engine->Graphics()->ExecuteCommandBuffer(*cmdBuffer);
}
