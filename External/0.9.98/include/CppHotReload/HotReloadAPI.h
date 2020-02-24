//
//  Copyright (c) 2014 - 2019 C++ Hot Reload SL. All rights reserved
//
//  This file is subject to the terms and conditions defined in
//  file 'LICENSE.txt', which is part of this source code package.
//

#pragma once

#if defined(_WIN32)
#   ifdef CPP_HOT_RELOAD_EXPORTS
#       define CPP_HOT_RELOAD_API __declspec(dllexport)
#   else
#       define CPP_HOT_RELOAD_API __declspec(dllimport)
#   endif
#   define CPP_HOT_RELOAD_DECLARE __cdecl
#   define CPP_HOT_RELOAD_API_HIDDEN
#elif defined(__APPLE__)
#   ifdef __APPLE__
#       include <TargetConditionals.h>
#       if TARGET_OS_IPHONE == 0
#           define CPP_HOT_RELOAD_API        __attribute__((visibility("default")))
#           define CPP_HOT_RELOAD_API_HIDDEN __attribute__((visibility("hidden")))
#           define CPP_HOT_RELOAD_DECLARE
#       endif
#   endif
#else
#	error "platform not supported in C++ Hot Reload"
#endif

#if defined(CPP_HOT_RELOAD_API)
#define CPP_HOT_RELOAD_VERSION_MAJOR 0
#define CPP_HOT_RELOAD_VERSION_MINOR 9
#define CPP_HOT_RELOAD_VERSION_REVISION 98
#include <cstddef>
#ifdef __cplusplus
extern "C"
{
#endif
	namespace CppHotReload
	{
		namespace Configuration
		{
			enum Prameter
			{
				//
				// AUTO, will try to use move operator and constructors and lately copy constructors and operators
				// PASSIVE, the user will capture the object and operate it
				//
				// Default: AUTO
				//
				NEW_OBJECT,	
				//
				// YES, construct the database in the first initialization and build all files in the given source 
				// code folder. Useful if you have no serialization or your engine, either doesn't compile the game
				// source code with the rest of the engine, or it's dependant of the C++ Hot Reload dependency 
				// graph or cached information
				// NO, use this option if your application at compile time build serialized objects and the game 
				// code.
				//
				// Default: NO
				//
				PARSE_ON_DEMAND,	
				//
				// YES, parsing on demand doesn't build the entire dependency graph, 
				// this will update dependencies and dependants when reload.
				// Default: YES
				//
				SEARCH_DEPENDENCIES_ON_DEMAND,	
				//
				// YES, limit the search of dependencies only when it's the first time we reload a file.
				// Default: NO
				//
				SEARCH_DEPENDENCIES_ON_DEMAND_WHEN_V_0,	
				//
				// YES, C++ Hot Reload will not be available until this process finish.
				// Available since v0.9.98:
				// Note: This process is not yet multi-threaded
				// Default: NO
				//
				BUILD_DEPENDENCY_GRAPH_ON_BACKGROUND,	
				//
				// This file will be included once before the source code generated, allow you to create your own hooks for your engine
				//
				HOOK_FILE,		
				//
				// Callback in the user space to create and delete the new pointer
				//
				DATA_FUNCTION,		// This function will be called and passed as void* in  DefaultNew(T*& NewClass, T*& OldClass, const char* varName, void* userData)
				DEFAULT_NEW,
				DEFAULT_DELETE,	
			};

			typedef void*(CPP_HOT_RELOAD_DECLARE* DATA_USER_FUNCTION) (void);
			typedef void (CPP_HOT_RELOAD_DECLARE* DEFAULT_USER_NEW)   (void*& dst, void*& src, const char* const typeName);
			typedef void (CPP_HOT_RELOAD_DECLARE* DEFAULT_USER_DELETE)(void*& ptr, const char* const typeName);

			struct CPP_HOT_RELOAD_API Option
			{
				Prameter param;
				const char* value;
			};
			struct CPP_HOT_RELOAD_API CallBacks
			{
				Prameter param;
				void* value;
			};
			//
			// Variables to be replaced after include files using $(variable)
			//
			struct CPP_HOT_RELOAD_API EnvironmentVariable
			{
				const char* key;
				const char* value;
			};
		}

		namespace Compiler
		{
			enum Prameter
			{
				COMPILER_EXEC,
				ARCHITECTURE,
				IS_DEBUG,
				EXTRA_MACROS,
				SET_MACROS,
				SET_OPTIONS,
				CPP_LANGUAGE_DIALECT,

				// OSX
				CPP_LANGUAGE,
				CPP_STANDARD_LIBRARIES,
				BASE_SDK,
				OS_MIN_VERSION,

				// Windows
				VS_VARS_ALL,
				FLOATING_POINT_BEHAVIOR,
				RUN_TIME_LIBRARY_DEBUG,
				RUN_TIME_LIBRARY_RELEASE,
				SHARED_PDB,
				SHARED_IDB,

				// Additional Libraries to link with
				ADDITIONAL_LIB_PATHS,
				ADDITIONAL_LIBRARIES,

				// UE4 only
				UE4_PROJECT_GENERATED_INCLUDES,
			};

			struct CPP_HOT_RELOAD_API Option
			{
				Prameter param;
				const char* value;
			};
		}

		namespace Linker
		{
			enum Prameter
			{
				LINKER_EXEC,
				ARCHITECTURE,
				IS_DEBUG,
				SET_OPTIONS,
				LINK_WITH_EXEC,

				// Windows
				VS_VARS_ALL,

				// Additional Libraries to link with
				ADDITIONAL_LIB_PATHS,
				ADDITIONAL_LIBRARIES,
			};

			struct CPP_HOT_RELOAD_API Option
			{
				Prameter param;
				const char* value;
			};
		}

		struct CPP_HOT_RELOAD_API TypeFileInfo
		{
			char** baseTypes;
			size_t baseTypesSize;
			char*  typeName;
			char*  moduleName;
		};

		typedef void (CPP_HOT_RELOAD_DECLARE* CPP_HOT_RELOAD_SAFE_CALL)(void);
		struct CPP_HOT_RELOAD_API RecoveryGuard
		{
			static void Execute(CPP_HOT_RELOAD_SAFE_CALL fn);
		};

		//
		// Forward declaration of the Serialization class
		//
		struct VarManager;
		// CallBack when a new pointer is ready
		typedef void  (CPP_HOT_RELOAD_DECLARE* CPP_HOT_RELOAD_CALLBACK_BEGIN_RELOAD)(const char* const fileThatTriggeredReload, const char*& commandToExecuteBeforeReload);
		typedef void  (CPP_HOT_RELOAD_DECLARE* CPP_HOT_RELOAD_CALLBACK_END_RELOAD)(void);
		typedef void* (CPP_HOT_RELOAD_DECLARE* CPP_HOT_RELOAD_CALLBACK_NEW)(size_t size);
		typedef void  (CPP_HOT_RELOAD_DECLARE* CPP_HOT_RELOAD_CALLBACK_DELETE)(void* ptr);
		typedef void  (CPP_HOT_RELOAD_DECLARE* CPP_HOT_RELOAD_CALLBACK_DID_INSTANCE_RELOAD)
			(const char* const typeName, void*& ptr, const char* const uuid);
		typedef void  (CPP_HOT_RELOAD_DECLARE* CPP_HOT_RELOAD_CALLBACK_DID_VARMANAGER_RELOAD)
			(VarManager* varManager, void*& ptr);
		typedef void  (CPP_HOT_RELOAD_DECLARE* CPP_HOT_RELOAD_CALLBACK_FATAL_ERROR)(const char* const typeName);
		typedef void  (CPP_HOT_RELOAD_DECLARE* CPP_HOT_RELOAD_CALLBACK_FILTER_FILE_TO_BUILD)(const char* const filePath, bool& hasToBuild);
		typedef void  (CPP_HOT_RELOAD_DECLARE* CPP_HOT_RELOAD_CALLBACK_WILL_COMPILE_FILE)   (const char* const filePath, const char*& additionalFilesToCompile, bool& addAdditionalFilesToMainLibrary, const char*& addAdditionalOptionsToCompile);
		typedef void  (CPP_HOT_RELOAD_DECLARE* CPP_HOT_RELOAD_CALLBACK_DID_COMPILE_FILE)    (const char* const filePath, bool wasOk);
		typedef void  (CPP_HOT_RELOAD_DECLARE* CPP_HOT_RELOAD_CALLBACK_WILL_LINK_FILE)      (const char* const filePath, const char*& additionalObjsToLink);
		typedef void  (CPP_HOT_RELOAD_DECLARE* CPP_HOT_RELOAD_CALLBACK_DID_LINK_FILE)       (VarManager* vmFile, bool wasOk);
		typedef void  (CPP_HOT_RELOAD_DECLARE* CPP_HOT_RELOAD_CALLBACK_DID_ADD_TYPE)		(TypeFileInfo* scriptFile);
		typedef void  (CPP_HOT_RELOAD_DECLARE* CPP_HOT_RELOAD_CALLBACK_DID_REMOVE_TYPE)		(TypeFileInfo* scriptFile);

		typedef void  (CPP_HOT_RELOAD_DECLARE* CPP_HOT_RELOAD_CALLBACK_LOG_FATAL)   (const char* msg);
		typedef void  (CPP_HOT_RELOAD_DECLARE* CPP_HOT_RELOAD_CALLBACK_LOG_CRITICAL)(const char* msg);
		typedef void  (CPP_HOT_RELOAD_DECLARE* CPP_HOT_RELOAD_CALLBACK_LOG_ERROR)   (const char* msg);
		typedef void  (CPP_HOT_RELOAD_DECLARE* CPP_HOT_RELOAD_CALLBACK_LOG_WARNING) (const char* msg);
		typedef void  (CPP_HOT_RELOAD_DECLARE* CPP_HOT_RELOAD_CALLBACK_LOG_SYSTEM)  (const char* msg);

		CPP_HOT_RELOAD_API void CPP_HOT_RELOAD_DECLARE Create(const char**   scriptDirs,
															  unsigned short scriptDirsSize,
															  const char**   includeDirs,
															  unsigned short includeDirsSize,
															  Configuration::Option configurationOptions[],	unsigned short configurationOptionsSize,
															  Configuration::CallBacks callbacksOptions[], unsigned short callbacksOptionsSize,
															  Compiler::Option compilerOptions[], unsigned short compilerOptionsSize,
															  Linker::Option linkerOptions[], unsigned short linkerOptionsSize,
															  const char* const libraryDir,
															  const char* const guid);
		
		CPP_HOT_RELOAD_API void CPP_HOT_RELOAD_DECLARE CreateWithConfigurationFiles(const char**   scriptDirs,
																					unsigned short scriptDirsSize,
																					const char* const includeFile,
																					Configuration::EnvironmentVariable environmentVariables[], unsigned short environmentVariablesSize,
																					Configuration::CallBacks callbacksOptions[], unsigned short callbacksOptionsSize,
																					Configuration::Option configurationOptions[], unsigned short configurationOptionsSize,
																					Compiler::Option compilerOptions[], unsigned short compilerOptionsSize,
																					Linker::Option linkerOptions[], unsigned short linkerOptionsSize,
																					const char* const libraryDir,
																					const char* const guid);
		
		CPP_HOT_RELOAD_API void CPP_HOT_RELOAD_DECLARE ConfigureMemoryAllocation(CPP_HOT_RELOAD_CALLBACK_NEW fnCallBackNew, CPP_HOT_RELOAD_CALLBACK_DELETE fnCallBackDelete);

		CPP_HOT_RELOAD_API void CPP_HOT_RELOAD_DECLARE ConfigureCallBacks(
			CPP_HOT_RELOAD_CALLBACK_BEGIN_RELOAD		  fnCallBackBeginReload,
			CPP_HOT_RELOAD_CALLBACK_END_RELOAD		      fnCallBackEndReload,
			CPP_HOT_RELOAD_CALLBACK_FILTER_FILE_TO_BUILD  fnCallBackFilterFileToBuild,
			CPP_HOT_RELOAD_CALLBACK_WILL_COMPILE_FILE     fnCallBackWillCompileFile,
			CPP_HOT_RELOAD_CALLBACK_DID_COMPILE_FILE      fnCallBackDidCompileFile,
			CPP_HOT_RELOAD_CALLBACK_WILL_LINK_FILE        fnCallBackWillLinkFile,
			CPP_HOT_RELOAD_CALLBACK_DID_LINK_FILE         fnCallBackDidLinkFile,
			CPP_HOT_RELOAD_CALLBACK_DID_ADD_TYPE          fnCallBackDidAddType,
			CPP_HOT_RELOAD_CALLBACK_DID_REMOVE_TYPE       fnCallBackDidRemoveType,
			CPP_HOT_RELOAD_CALLBACK_DID_INSTANCE_RELOAD   fnCallBackDidInstanceReload,
			CPP_HOT_RELOAD_CALLBACK_DID_VARMANAGER_RELOAD fnCallBackDidVarManagerReload,
			CPP_HOT_RELOAD_CALLBACK_FATAL_ERROR           fnCallBackFatalError);

		CPP_HOT_RELOAD_API void CPP_HOT_RELOAD_DECLARE ConfigureLogCallBacks(
			CPP_HOT_RELOAD_CALLBACK_LOG_FATAL    fnCallBackLogFatal, 
			CPP_HOT_RELOAD_CALLBACK_LOG_CRITICAL fnCallBackLogCritical,
			CPP_HOT_RELOAD_CALLBACK_LOG_ERROR    fnCallBackLogError, 
			CPP_HOT_RELOAD_CALLBACK_LOG_WARNING  fnCallBackLogWarning, 
			CPP_HOT_RELOAD_CALLBACK_LOG_SYSTEM   fnCallBackLogSystem);

		CPP_HOT_RELOAD_API void CPP_HOT_RELOAD_DECLARE ActivateProtection(void);
		CPP_HOT_RELOAD_API void CPP_HOT_RELOAD_DECLARE DeactivateProtection(void);
		CPP_HOT_RELOAD_API void CPP_HOT_RELOAD_DECLARE Restore(void);
		CPP_HOT_RELOAD_API void CPP_HOT_RELOAD_DECLARE Destroy(void);

		CPP_HOT_RELOAD_API bool CPP_HOT_RELOAD_DECLARE IsInitialized(void);
		CPP_HOT_RELOAD_API void CPP_HOT_RELOAD_DECLARE Update   (void);
		CPP_HOT_RELOAD_API bool CPP_HOT_RELOAD_DECLARE IsWorking(void);
		CPP_HOT_RELOAD_API bool CPP_HOT_RELOAD_DECLARE HasType(const char* const typeName);

		CPP_HOT_RELOAD_API void   CPP_HOT_RELOAD_DECLARE Save(const char* const typeName, void*& data, const char* uuid);
		CPP_HOT_RELOAD_API size_t CPP_HOT_RELOAD_DECLARE Load(const char* const typeName, void*& data, const char* uuid);
		CPP_HOT_RELOAD_API void   CPP_HOT_RELOAD_DECLARE ReloadVarManager(const char* const typeName, void*& data, const char*  uuid);
		CPP_HOT_RELOAD_API VarManager* CPP_HOT_RELOAD_DECLARE GetVarManager(const char* const typeName, const char* uuid);
		CPP_HOT_RELOAD_API void   CPP_HOT_RELOAD_DECLARE Serialize(VarManager* varManager, const char* const idUnique);
		CPP_HOT_RELOAD_API size_t CPP_HOT_RELOAD_DECLARE NewInstance(const char* const typeName, void*& data, const char*& uuid);
		CPP_HOT_RELOAD_API void   CPP_HOT_RELOAD_DECLARE DeleteInstance(const char* const typeName, void*& data, const char*& uuid, bool isCopy = false);
		CPP_HOT_RELOAD_API const char* CPP_HOT_RELOAD_DECLARE RegisterPtr(const char* const typeName, void* data, const char* const idCustom = nullptr);
		CPP_HOT_RELOAD_API void   CPP_HOT_RELOAD_DECLARE UnregisterPtr(const char* const typeName, void* data, const char* const idCustom = nullptr);
		CPP_HOT_RELOAD_API bool   CPP_HOT_RELOAD_DECLARE RestoreData(const char* const typeName, void* from, void* to);
		CPP_HOT_RELOAD_API void*  CPP_HOT_RELOAD_DECLARE GetPtrById(const char* const typeName, const char* const uuid);
		CPP_HOT_RELOAD_API void   CPP_HOT_RELOAD_DECLARE NewPtrByFile(const char* const typeName, void*& data, size_t size, const char* const file, const char*& uuid);
		CPP_HOT_RELOAD_API void	  CPP_HOT_RELOAD_DECLARE DeletePtrByFile(const char* const typeName, void*& data, const char* const file);
		CPP_HOT_RELOAD_API void   CPP_HOT_RELOAD_DECLARE Purge(void);
		CPP_HOT_RELOAD_API void   CPP_HOT_RELOAD_DECLARE DeleteOldLibraries(void);
		//
		// 2014 - "The elder function" of C++ Hot Reload to replace virtual tables and fast swap classes, legit function but try to avoid it,
		// re-creating the objects to follow the engine registrations of new objects
		//
		CPP_HOT_RELOAD_API bool   CPP_HOT_RELOAD_DECLARE ReplaceVirtualTable(void*&, void*&, size_t);
	}
#ifdef __cplusplus
}
#endif
#endif // CPP_HOT_RELOAD_API
