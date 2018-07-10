#include "TestBasic.h"

SG::TestBasic::TestBasic(SG::SGEngine2D* toUse) : TestBase(toUse)
{
	scene = engine->CreateScene(0, 10, SG::SGGuid("testBasicScene"));
	entHandles.push_back(scene->GetEntityManager().CreateEntity());
	entHandles.push_back(scene->GetEntityManager().CreateEntity());
	entHandles.push_back(scene->GetEntityManager().CreateEntity());
	entHandles.push_back(scene->GetEntityManager().CreateEntity());
	entHandles.push_back(scene->GetEntityManager().CreateEntity());
	entHandles.push_back(scene->GetEntityManager().CreateEntity());

	engine->Transform()->CreateParentChildRelation(entHandles[0]);
	engine->Transform()->CreateParentChildRelation(entHandles[1]);
	engine->Transform()->Move(entHandles[1], -200.0, -20.0);
	engine->Transform()->CreateParentChildRelation(entHandles[2]);
	engine->Transform()->Move(entHandles[2], 200.0, -20.0);
	engine->Transform()->BindChild(entHandles[0], entHandles[1]);
	engine->Transform()->BindChild(entHandles[0], entHandles[2]);
	engine->Transform()->CreateParentChildRelation(entHandles[3]);
	engine->Transform()->Move(entHandles[3], -250.0, -25.0);
	engine->Transform()->CreateParentChildRelation(entHandles[4]);
	engine->Transform()->Move(entHandles[4], 250.0, -25.0);
	engine->Transform()->BindChild(entHandles[1], entHandles[3]);
	engine->Transform()->BindChild(entHandles[2], entHandles[4]);
	engine->Transform()->Move(entHandles[5], 0, 300);

	engine->Graphics()->ShapeManager()->CreateRectangleShape(entHandles[0], 100.0f, 100.0f);
	engine->Graphics()->ShapeManager()->CreateRectangleShape(entHandles[1], 50.0f, 50.0f);
	engine->Graphics()->ShapeManager()->CreateRectangleShape(entHandles[2], 50.0f, 50.0f);
	engine->Graphics()->ShapeManager()->CreateRectangleShape(entHandles[3], 25.0f, 25.0f);
	engine->Graphics()->ShapeManager()->CreateRectangleShape(entHandles[4], 25.0f, 25.0f);
	engine->Graphics()->ShapeManager()->CreateRectangleShape(entHandles[5], 100.0f, 100.0f);

	auto texPath = std::string("C:/Users/Joakim/Pictures/Brock_Stadium.png");
	engine->Graphics()->TextureManager()->LoadTexture(entHandles[0], texPath);
	engine->Graphics()->TextureManager()->SetTexture(entHandles[5], SG::SGGuid("C:/Users/Joakim/Pictures/Brock_Stadium.png"));

	camera.xPos = 0;
	camera.yPos = 0;
	camera.width = 1280;
	camera.height = 720;

	viewPort.top = 0.0f;
	viewPort.left = 0.0f;
	viewPort.width = 0.5f;
	viewPort.height = 0.5f;

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

	SG::ShaderBinding shaderBinding2[3];
	SG::ShaderDataSourceType shaderSource2[3];
	shaderBinding2[0].dataType = SG::ShaderDataType::SHADER_TRANSFORM;
	shaderBinding2[0].identifier;
	shaderBinding2[0].nameOnShader = "world";
	shaderSource2[0] = SG::ShaderDataSourceType::HANDLER;
	shaderBinding2[1].dataType = SG::ShaderDataType::SHADER_4X4;
	shaderBinding2[1].identifier = SG::SGGuid("ProjectionMatrix");
	shaderBinding2[1].nameOnShader = "projection";
	shaderSource2[1] = SG::ShaderDataSourceType::GLOBAL;
	shaderBinding2[2].dataType = SG::ShaderDataType::SHADER_FLOAT;
	shaderBinding2[2].identifier = SG::SGGuid("gradient");
	shaderBinding2[2].nameOnShader = "gradient";
	shaderSource2[2] = SG::ShaderDataSourceType::PRIVATE;

	engine->Graphics()->ShaderManager()->CreateRenderPipeline(SG::SGGuid("TestPipeline"), shaderBinding, shaderSource, 3, "E:/VS Projects/SteelGearGraphics2D/BlastOff/Shaders/vsTest.txt", "", "E:/VS Projects/SteelGearGraphics2D/BlastOff/Shaders/psTest.txt");
	engine->Graphics()->ShaderManager()->SetRenderPipelineOfEntity(entHandles[0], SG::SGGuid("TestPipeline"));
	engine->Graphics()->ShaderManager()->CreateRenderPipeline(SG::SGGuid("TestPipeline2"), shaderBinding2, shaderSource2, 3, "E:/VS Projects/SteelGearGraphics2D/BlastOff/Shaders/vsTest.txt", "", "E:/VS Projects/SteelGearGraphics2D/BlastOff/Shaders/psTest2.txt");
	engine->Graphics()->ShaderManager()->SetRenderPipelineOfEntity(entHandles[1], SG::SGGuid("TestPipeline2"));
	engine->Graphics()->ShaderManager()->SetRenderPipelineOfEntity(entHandles[2], SG::SGGuid("TestPipeline2"));
	engine->Graphics()->ShaderManager()->SetRenderPipelineOfEntity(entHandles[3], SG::SGGuid("TestPipeline2"));
	engine->Graphics()->ShaderManager()->SetRenderPipelineOfEntity(entHandles[4], SG::SGGuid("TestPipeline2"));
	engine->Graphics()->ShaderManager()->SetRenderPipelineOfEntity(entHandles[5], SG::SGGuid("TestPipeline"));

	engine->Graphics()->ShaderManager()->UpdateGlobalData(SG::SGGuid("ProjectionMatrix"), engine->Graphics()->GetProjectionMatrix(camera));

	SG::SGGuid commandGuids[6] = { SG::SGGuid("clear"), SG::SGGuid("render1"), SG::SGGuid("render2"), SG::SGGuid("render3"), SG::SGGuid("render4"), SG::SGGuid("display") };
	cmdBuffer = new SG::SGCommandBuffer2D(6, commandGuids);

	auto clearJob = engine->Graphics()->CreateGraphicsJob(SG::SGGuid("clear"), engine->Graphics(), &SG::SGRenderer2D::ClearRenderTarget, SG::SGColour(0, 0, 255));
	cmdBuffer->SetGraphicsJob(clearJob);

	viewPort.top = 0.0f;
	viewPort.left = 0.0f;
	auto renderJob1 = engine->Graphics()->CreateGraphicsJob(SG::SGGuid("render1"), engine->Graphics(), &SG::SGRenderer2D::RenderScene, scene->GetSceneEntities(), camera, viewPort);
	cmdBuffer->SetGraphicsJob(renderJob1);
	viewPort.top = 0.0f;
	viewPort.left = 0.5f;
	auto renderJob2 = engine->Graphics()->CreateGraphicsJob(SG::SGGuid("render2"), engine->Graphics(), &SG::SGRenderer2D::RenderScene, scene->GetSceneEntities(), camera, viewPort);
	cmdBuffer->SetGraphicsJob(renderJob2);
	viewPort.top = 0.5f;
	viewPort.left = 0.0f;
	auto renderJob3 = engine->Graphics()->CreateGraphicsJob(SG::SGGuid("render3"), engine->Graphics(), &SG::SGRenderer2D::RenderScene, scene->GetSceneEntities(), camera, viewPort);
	cmdBuffer->SetGraphicsJob(renderJob3);
	viewPort.top = 0.5f;
	viewPort.left = 0.5f;
	auto renderJob4 = engine->Graphics()->CreateGraphicsJob(SG::SGGuid("render4"), engine->Graphics(), &SG::SGRenderer2D::RenderScene, scene->GetSceneEntities(), camera, viewPort);
	cmdBuffer->SetGraphicsJob(renderJob4);

	auto displayJob = engine->Graphics()->CreateGraphicsJob(SG::SGGuid("display"), engine->Graphics(), &SG::SGRenderer2D::Display);
	cmdBuffer->SetGraphicsJob(displayJob);
}

SG::TestBasic::~TestBasic()
{
	engine->DestroyScene(scene);
	delete cmdBuffer;
}

void SG::TestBasic::Update(float dt)
{
	static int dir = 1;
	static int counter = 0;
	static int superCounter = 0;
	static float movement = 0.0f;

	engine->Transform()->Move(entHandles[0], 0 * dir, 0.0 * dir);
	engine->Transform()->Rotate(entHandles[0], 0.001);
	engine->Transform()->Rotate(entHandles[1], 0.001);
	engine->Transform()->Rotate(entHandles[2], 0.001);

	movement += 0.1;
	//camera.xPos = movement;
	engine->Graphics()->ShaderManager()->UpdatePrivateData(SG::SGGuid("gradient"), entHandles[1], (superCounter % 501) / 500.0f);
	engine->Graphics()->ShaderManager()->UpdatePrivateData(SG::SGGuid("gradient"), entHandles[2], (superCounter % 1001) / 1000.0f);
	engine->Graphics()->ShaderManager()->UpdatePrivateData(SG::SGGuid("gradient"), entHandles[3], (superCounter % 1501) / 1500.0f);
	engine->Graphics()->ShaderManager()->UpdatePrivateData(SG::SGGuid("gradient"), entHandles[4], (superCounter % 2001) / 2000.0f);


	viewPort.top = 0.0f;
	viewPort.left = 0.0f;
	auto renderJob1 = engine->Graphics()->CreateGraphicsJob(SG::SGGuid("render1"), engine->Graphics(), &SG::SGRenderer2D::RenderScene, scene->GetSceneEntities(), camera, viewPort);
	cmdBuffer->SetGraphicsJob(renderJob1);
	viewPort.top = 0.0f;
	viewPort.left = 0.5f;
	auto renderJob2 = engine->Graphics()->CreateGraphicsJob(SG::SGGuid("render2"), engine->Graphics(), &SG::SGRenderer2D::RenderScene, scene->GetSceneEntities(), camera, viewPort);
	cmdBuffer->SetGraphicsJob(renderJob2);
	viewPort.top = 0.5f;
	viewPort.left = 0.0f;
	auto renderJob3 = engine->Graphics()->CreateGraphicsJob(SG::SGGuid("render3"), engine->Graphics(), &SG::SGRenderer2D::RenderScene, scene->GetSceneEntities(), camera, viewPort);
	cmdBuffer->SetGraphicsJob(renderJob3);
	viewPort.top = 0.5f;
	viewPort.left = 0.5f;
	auto renderJob4 = engine->Graphics()->CreateGraphicsJob(SG::SGGuid("render4"), engine->Graphics(), &SG::SGRenderer2D::RenderScene, scene->GetSceneEntities(), camera, viewPort);
	cmdBuffer->SetGraphicsJob(renderJob4);

	counter++;
	superCounter++;

	if (counter == 500)
	{
		dir *= -1;
		counter = 0;
	}

}

void SG::TestBasic::Render()
{
	engine->Graphics()->ExecuteCommandBuffer(*cmdBuffer);
}
