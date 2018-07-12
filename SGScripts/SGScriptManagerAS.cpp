#include "SGScriptManagerAS.h"

SG::SGScriptManagerAS::SGScriptManagerAS()
{
	engine = asCreateScriptEngine();
	int errorCode = 0;

	// Set the message callback to receive information on errors in human readable form.
	errorCode = engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);
	_ASSERT(errorCode >= 0);

	RegisterStdString(engine);

	ctx = engine->CreateContext();
}

SG::SGScriptManagerAS::~SGScriptManagerAS()
{
	ctx->Release();
	engine->ShutDownAndRelease();
}

SG::SGScriptHandle SG::SGScriptManagerAS::LoadScript(const char* file)
{
	BuiltScript temp;
	std::string stringModule = std::string("Module" + std::to_string(scripts.size()));
	temp.builder.StartNewModule(engine, stringModule.c_str());

	int error = temp.builder.AddSectionFromFile(file);
	if (error < 0)
	{
		// The builder wasn't able to load the file. Maybe the file
		// has been removed, or the wrong name was given, or some
		// preprocessing commands are incorrectly written.
		throw("Error when trying to load file");
	}

	error = temp.builder.BuildModule();
	if (error < 0)
	{
		// An error occurred. Instruct the script writer to fix the 
		// compilation errors that were listed in the output stream.
		throw("Error when trying to build file");
	}

	temp.mod = engine->GetModule(stringModule.c_str());
	scripts.push_back(temp);

	SGScriptHandle toReturn;
	toReturn.loadedScript = scripts.size() - 1;
	return toReturn;
}