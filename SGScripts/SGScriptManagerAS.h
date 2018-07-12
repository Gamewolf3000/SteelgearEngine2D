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

		template<typename T>
		void SetParameter(T* parameter, int nr)
		{
			ctx->SetArgObject(nr, parameter);
		}

		// This should be possible to get down to just one function using std::is_pod and std::enable_if_t
		template<>
		void SetParameter<int16_t>(int16_t* parameter, int nr)
		{
			ctx->SetArgAddress(nr, parameter);
		}
		template<>
		void SetParameter<uint16_t>(uint16_t* parameter, int nr)
		{
			ctx->SetArgAddress(nr, parameter);
		}
		template<>
		void SetParameter<int32_t>(int32_t* parameter, int nr)
		{
			ctx->SetArgAddress(nr, parameter);
		}
		template<>
		void SetParameter<uint32_t>(uint32_t* parameter, int nr)
		{
			ctx->SetArgAddress(nr, parameter);
		}
		template<>
		void SetParameter<int64_t>(int64_t* parameter, int nr)
		{
			ctx->SetArgAddress(nr, parameter);
		}
		template<>
		void SetParameter<uint64_t>(uint64_t* parameter, int nr)
		{
			ctx->SetArgAddress(nr, parameter);
		}
		template<>
		void SetParameter<float>(float* parameter, int nr)
		{
			ctx->SetArgAddress(nr, parameter);
		}
		template<>
		void SetParameter<double>(double* parameter, int nr)
		{
			ctx->SetArgAddress(nr, parameter);
		}
		template<>
		void SetParameter<char>(char* parameter, int nr)
		{
			ctx->SetArgAddress(nr, parameter);
		}
		template<>
		void SetParameter<unsigned char>(unsigned char* parameter, int nr)
		{
			ctx->SetArgAddress(nr, parameter);
		}
		template<>
		void SetParameter<bool>(bool* parameter, int nr)
		{
			ctx->SetArgAddress(nr, parameter);
		}

		template<typename T>
		void SetParameter(T parameter, int nr)
		{
			ctx->SetArgObject(nr, &parameter);
		}

		template<>
		void SetParameter<int16_t>(int16_t parameter, int nr)
		{
			ctx->SetArgWord(nr, parameter);
		}
		template<>
		void SetParameter<uint16_t>(uint16_t parameter, int nr)
		{
			ctx->SetArgWord(nr, parameter);
		}
		template<>
		void SetParameter<int32_t>(int32_t parameter, int nr)
		{
			ctx->SetArgDWord(nr, parameter);
		}
		template<>
		void SetParameter<uint32_t>(uint32_t parameter, int nr)
		{
			ctx->SetArgDWord(nr, parameter);
		}
		template<>
		void SetParameter<int64_t>(int64_t parameter, int nr)
		{
			ctx->SetArgQWord(nr, parameter);
		}
		template<>
		void SetParameter<uint64_t>(uint64_t parameter, int nr)
		{
			ctx->SetArgQWord(nr, parameter);
		}
		template<>
		void SetParameter<float>(float parameter, int nr)
		{
			ctx->SetArgFloat(nr, parameter);
		}
		template<>
		void SetParameter<double>(double parameter, int nr)
		{
			ctx->SetArgDouble(nr, parameter);
		}
		template<>
		void SetParameter<char>(char parameter, int nr)
		{
			ctx->SetArgByte(nr, parameter);
		}
		template<>
		void SetParameter<unsigned char>(unsigned char parameter, int nr)
		{
			ctx->SetArgByte(nr, parameter);
		}
		template<>
		void SetParameter<bool>(bool parameter, int nr)
		{
			ctx->SetArgByte(nr, parameter);
		}

	public:

		SGScriptManagerAS();
		~SGScriptManagerAS();

		SGScriptHandle LoadScript(const char* file);

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

		template <class... Args>
		void for_each_argument(Args&&... args) {
			int i = 0;
			int _[] = { (SetParameter(std::forward<Args>(args), i++),0)... };
		}

		//template <typename... Args>
		//inline void SetParameters(Args... parameters)
		//{
		//	for_each_argument(parameters...);
		//}

		template <typename... Args>
		void ExecuteScript(SGScriptHandle& script, const char* functionDeclaration, Args&&... parameters)
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

			//Prepare our context
			ctx->Prepare(func);

			for_each_argument(parameters...);

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

		template <>
		void ExecuteScript<>(SGScriptHandle& script, const char* functionDeclaration)
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

			//Prepare our context
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

	};

}