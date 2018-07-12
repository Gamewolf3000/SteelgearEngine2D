#pragma once

#include "../SGScripts/angelscript_2.32.0/sdk/angelscript/include/angelscript.h"
#include "../SGScripts/angelscript_2.32.0/sdk/add_on/scriptbuilder/scriptbuilder.h"
#include "../SGScripts/angelscript_2.32.0/sdk/add_on/scriptstdstring/scriptstdstring.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

namespace SG
{
	struct SGScriptHandle
	{
		friend class SGScriptManagerAS;
	private:
		unsigned int loadedScript;
	};

	struct BuiltScript
	{
		CScriptBuilder builder;
		asIScriptModule* mod;
	};

	class SGScriptManagerAS
	{
	private:

		asIScriptEngine * engine;

		std::unordered_map<std::string, bool> exposedClassTypes;
		std::unordered_map<std::string, bool> exposedClassObjects;
		std::unordered_map<std::string, bool> exposedClassFunctions;

		std::unordered_map<std::string, bool> exposedFunctions;

		std::vector<BuiltScript> scripts;
		asIScriptContext *ctx;

		template<typename T>
		static inline void Constructor(void *memory)
		{
			// Initialize the pre-allocated memory by calling the
			// object constructor with the placement-new operator
			new(memory) T();
		}

		template<typename T>
		static inline void Destructor(void *memory)
		{
			// Uninitialize the memory by calling the object destructor
			((T*)memory)->~T();
		}

		static void MessageCallback(asSMessageInfo* message)
		{
			std::cout << message->message << "\n";
		}

	public:

		SGScriptManagerAS()
		{
			engine = asCreateScriptEngine();
			int errorCode = 0;

			// Set the message callback to receive information on errors in human readable form.
			errorCode = engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);
			_ASSERT(errorCode >= 0);

			RegisterStdString(engine);

			ctx = engine->CreateContext();
		}

		~SGScriptManagerAS()
		{
			ctx->Release();
			engine->ShutDownAndRelease();
		}

		SGScriptHandle LoadScript(const char* file)
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


		void ExecuteScript(SGScriptHandle& script, const char* functionDeclaration)
		{
			if (script.loadedScript == -1)
				throw("Invalid ScriptHandle");

			asIScriptFunction *func = scripts[script.loadedScript].mod->GetFunctionByDecl(functionDeclaration);
			if (func == 0)
			{
				// The function couldn't be found. Instruct the script writer
				// to include the expected function in the script.
				throw("Error when trying to find the function declaration");
			}

			//Prepare our context, and then execute
			ctx->Prepare(func);
			int error = ctx->Execute();
			if (error != asEXECUTION_FINISHED)
			{
				// The execution didn't complete as expected. Determine what happened.
				if (error == asEXECUTION_EXCEPTION)
				{
					// An exception occurred, let the script writer know what happened so it can be corrected.
					throw((std::string("An exception ") + std::string(ctx->GetExceptionString()) + std::string(" has occurred!")).c_str());
				}

				throw("An error occured while executing the script");
			}
		}

		template <typename ReturnType, typename... Args>
		inline void ExposeFunction(ReturnType(ptr)(Args...), const char* declaration)
		{
			this->engine->RegisterGlobalFunction(declaration, asFUNCTIONPR(ptr, (Args...), ReturnType), asCALL_CDECL);
		}

		template<class objectClass>
		inline void ExposeClass(const char* className)
		{
			if (exposedClassTypes[std::string(className)])
			{
				return;
			}

			this->engine->RegisterObjectType(className, sizeof(objectClass), asOBJ_VALUE | asGetTypeTraits<objectClass>());
			this->engine->RegisterObjectBehaviour(className, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ScriptManager::Constructor<objectClass>), asCALL_CDECL_OBJLAST);
			this->engine->RegisterObjectBehaviour(className, asBEHAVE_DESTRUCT, "void f()", asFUNCTION(ScriptManager::Destructor<objectClass>), asCALL_CDECL_OBJLAST);

			exposedClassTypes[std::string(className)] = true;
		}

		template<class objectClass>
		inline void ExposeObject(const char* objectDeclaration, objectClass* object)
		{
			if (exposedClassObjects[std::string(objectDeclaration)])
			{
				return;
			}

			this->engine->RegisterGlobalProperty(objectDeclaration, object);

			exposedClassObjects[std::string(objectDeclaration)] = true;
		}

		template <class objectClass, typename ReturnType, typename... Args>
		inline void ExposeObjectMethod(ReturnType(objectClass::*ptr)(Args...), const char* className, const char* functionDeclaration)
		{
			asSFuncPtr p(3);
			p.CopyMethodPtr(&ptr, SINGLE_PTR_SIZE);

			this->engine->RegisterObjectMethod(className, functionDeclaration, p, asCALL_THISCALL);
		}


	};

}