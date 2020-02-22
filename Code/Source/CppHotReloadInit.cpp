//
//  Copyright (c) since 2014 - 2020 C++ Hot Reload SL. All rights reserved
//

#include "StdAfx.h"
//
// C/C++ Hot Reload
//
#include "CppHotReloadInit.h"
#include "CppHotReloadConfig.h"
#include "CppHotReloadSubscriber.h"
//
// C/C++
//
#include <stdio.h>
#include <vector>

namespace CppHotReload
{
	//
	// Array size
	//
	using Size = ptrdiff_t;

	template<typename T, Size n>
	Size countOf(T(&)[n]) { return n; }

    //
    // Initialization method of C++ Hot Reload / Scripting
    //
    void InitHotReload()
    {
		//
		// Let's get first at runtime the proper paths of the application
		//
		LocateProjectPaths();
        //
        // C++ Hot Reload license key - Community
        //
        const std::string& licenseGuid = "1a2a4b66-b878-43a3-8308-39b24bd41106";
        //
        // Prepare pre-processor macros
        //
#if CPP_HOT_RELOAD_PLATFORM_WINDOWS
		//
		// Libraries directory
		//
		std::vector<std::string> lyLibPaths;
		lyLibPaths.emplace_back("/Code/Framework/AzCore/AzCore");
		lyLibPaths.emplace_back("/Code/Framework/AzFramework/AzFramework");
		lyLibPaths.emplace_back("/Code/Framework/AzGameFramework/AzGameFramework");
		lyLibPaths.emplace_back("/Code/Framework/GridMate");
		lyLibPaths.emplace_back("/Gems/CryLegacy/Code");

		std::string additionalLibPaths;
		for (const std::string& libPath : lyLibPaths)
		{
			additionalLibPaths += "/LIBPATH:\"" + GetBinTempDir() + libPath + "\" ";
		}
		//
		// 3rd party
		//
		additionalLibPaths += "/LIBPATH:\"" + GetThirdPartyDir() + "/Lua/5.1.1.9-az/build/win_x64/vc140/release\" ";
		additionalLibPaths += "/LIBPATH:\"" + GetThirdPartyDir() + "/zstd/1.35-pkg.1/build/win_x64/vc140/release\" ";
		additionalLibPaths += "/LIBPATH:\"" + GetThirdPartyDir() + "/lz4/r128-pkg.3/build/win_x64/vc140/release\" ";
		additionalLibPaths += "/LIBPATH:\"" + GetThirdPartyDir() + "/zlib/1.2.8-pkg.3/build/win_x64/vc140/release\" ";
		//
		// Libraries
		// TODO: link with the hot reload lib need to be gone in the future
		//
		std::string additionalLibs = "AzGameFramework.lib AzFramework.lib GridMate.lib GridMateForTools.lib CryAction_AutoFlowNode.lib AzCore.lib lua.lib zstd.lib lz4.lib zlib.lib User32.lib Advapi32.lib PsAPI.lib User32.lib Advapi32.lib PsAPI.lib bcrypt.lib gdi32.lib ";
#elif CPP_HOT_RELOAD_PLATFORM_MAC
#endif
        Compiler::Option CompilerOptions[] =
        {
			{ Compiler::Prameter::OS_MIN_VERSION		, "10.13.0" },
			{ Compiler::Prameter::CPP_LANGUAGE			, "objective-c++" },
			{ Compiler::Prameter::CPP_LANGUAGE_DIALECT	, "c++14" },
			{ Compiler::Prameter::ARCHITECTURE			, CPP_HOT_RELOAD_ARCH_NAME },
			{ Compiler::Prameter::SET_MACROS			, GetCppHotReloadPreprocessorOptions().c_str() },
			{ Compiler::Prameter::SET_OPTIONS			, GetCppHotReloadCompileOptions().c_str() },
#if CPP_HOT_RELOAD_IS_DEBUG
			{ Compiler::Prameter::IS_DEBUG, "YES" },
#endif
#if CPP_HOT_RELOAD_PLATFORM_WINDOWS
			{ Compiler::Prameter::VS_VARS_ALL, CPP_HOT_RELOAD_VS_VAR_ALL },
#endif
        };

		Linker::Option LinkerOptions[] =
        {
			{ Linker::Prameter::SET_OPTIONS			, GetCppHotReloadLinkOptions().c_str() },
			{ Linker::Prameter::ADDITIONAL_LIB_PATHS, additionalLibPaths.c_str() },
			{ Linker::Prameter::ADDITIONAL_LIBRARIES, additionalLibs.c_str() },
#if CPP_HOT_RELOAD_IS_DEBUG
			{ Linker::Prameter::IS_DEBUG, "YES" },
#endif
#if CPP_HOT_RELOAD_PLATFORM_WINDOWS
			{ Linker::Prameter::VS_VARS_ALL, CPP_HOT_RELOAD_VS_VAR_ALL },
#endif
        };
		//
		// Ly C++ Hot Reload configuration
		//
		Configuration::Option ConfigurationOptions[] =
		{
			{ Configuration::Prameter::NEW_OBJECT, "AUTO" },
			{ Configuration::Prameter::PARSE_ON_DEMAND, "YES" },
			{ Configuration::Prameter::HOOK_FILE, GetCppHotReloadHookFileName().c_str() },
#if CPP_HOT_RELOAD_DEPENDENCY_TEST
			{ Configuration::Prameter::SEARCH_DEPENDENCIES_ON_DEMAND, "YES" },			// I don't use always EBuses :(
			{ Configuration::Prameter::SEARCH_DEPENDENCIES_ON_DEMAND_FAST, "YES" },		// dependencies are found if are .h or .cpp only
			{ Configuration::Prameter::SEARCH_DEPENDENCIES_ON_DEMAND_WHEN_V_0, "NO" },	// NO, search always dependencies
#else
			{ Configuration::Prameter::SEARCH_DEPENDENCIES_ON_DEMAND, "NO" },			// I always use EBuses!
#endif
		};
		//
		// C++ Hot Reload environment variables to replace after read the files, like include directories txt
		//
		Configuration::EnvironmentVariable EnvironmentVariables[] =
		{
			{ "DevDir"		 , GetDevDir().c_str()		  },
			{ "ThirdPartyDir", GetThirdPartyDir().c_str() },
			{ "BinTemp"		 , GetBinTempDir().c_str()    },
		};
		Configuration::CallBacks CallBacks[] =
		{
			{ Configuration::Prameter::DATA_FUNCTION  , CppHotReload::GetGlobalEnvironment },
			{ Configuration::Prameter::DEFAULT_NEW	  , nullptr },
			{ Configuration::Prameter::DEFAULT_DELETE , nullptr },
		};
        //
        // HotReload initialization
        //
		//
		// C-Style games dir
		//
		const std::vector<std::string>& gameDirs = GetGameDirs();
		std::vector<const char*> filesDirectories;
		filesDirectories.reserve(gameDirs.size());
		std::transform(std::begin(gameDirs), std::end(gameDirs), std::back_inserter(filesDirectories), std::mem_fn(&std::string::c_str));

		CppHotReload::CreateWithConfigurationFiles( filesDirectories.data(), static_cast<unsigned short>(filesDirectories.size())
												  , GetCppHotReloadIncludeFiles().c_str()
												  , EnvironmentVariables, static_cast<unsigned short>(countOf(EnvironmentVariables))
												  , CallBacks			, static_cast<unsigned short>(countOf(CallBacks))
												  , ConfigurationOptions, static_cast<unsigned short>(countOf(ConfigurationOptions))
												  , CompilerOptions     , static_cast<unsigned short>(countOf(CompilerOptions))
												  , LinkerOptions       , static_cast<unsigned short>(countOf(LinkerOptions))
												  , GetCppHotReloadIntermediateDir().c_str(), licenseGuid.c_str());
    }
	//
	// Ly Memory Redirection 
	//
	void* Ly_Malloc(size_t size)
	{
		return azmalloc(size);
	}

	void Ly_Free(void* ptr)
	{
		azfree(ptr);
	}
    //
    // Now that the engine is initialized, before do more its initialise the library
    //
	void Init(
		CPP_HOT_RELOAD_CALLBACK_BEGIN_RELOAD		  fnBeginReload,
		CPP_HOT_RELOAD_CALLBACK_END_RELOAD			  fnEndReload,
		CPP_HOT_RELOAD_CALLBACK_FILTER_FILE_TO_BUILD  fnCallBackFilterToBuild,
		CPP_HOT_RELOAD_CALLBACK_WILL_COMPILE_FILE     fnCallBackWillCompileFile,
		CPP_HOT_RELOAD_CALLBACK_DID_COMPILE_FILE      fnCallBackDidCompileFile,
		CPP_HOT_RELOAD_CALLBACK_WILL_LINK_FILE        fnCallBackWillLinkFile,
		CPP_HOT_RELOAD_CALLBACK_DID_LINK_FILE         fnCallBackDidLinkFile,
		CPP_HOT_RELOAD_CALLBACK_DID_ADD_TYPE          fnCallBackDidAddType,
		CPP_HOT_RELOAD_CALLBACK_DID_REMOVE_TYPE       fnCallBackDidRemoveType,
		CPP_HOT_RELOAD_CALLBACK_DID_INSTANCE_RELOAD   fnCallBackDidInstanceReload,
		CPP_HOT_RELOAD_CALLBACK_DID_VARMANAGER_RELOAD fnCallBackDidVarManagerReload,
		CPP_HOT_RELOAD_CALLBACK_FATAL_ERROR           fnCallBackFatalError,
	    CPP_HOT_RELOAD_CALLBACK_LOG_FATAL 		fnLogFatal,
	    CPP_HOT_RELOAD_CALLBACK_LOG_CRITICAL 	fnLogCritical,
	    CPP_HOT_RELOAD_CALLBACK_LOG_ERROR 		fnLogError,
	    CPP_HOT_RELOAD_CALLBACK_LOG_WARNING 	fnLogWarning,
	    CPP_HOT_RELOAD_CALLBACK_LOG_SYSTEM 		fnLogSystem)
	{
		// C/C++ HotReload allocations
		CppHotReload::ConfigureMemoryAllocation(Ly_Malloc, Ly_Free);
		CppHotReload::ConfigureLogCallBacks(fnLogFatal, fnLogCritical, fnLogError, fnLogWarning, fnLogSystem);
		CppHotReload::ConfigureCallBacks(fnBeginReload, fnEndReload, fnCallBackFilterToBuild, fnCallBackWillCompileFile, fnCallBackDidCompileFile, fnCallBackWillLinkFile, fnCallBackDidLinkFile,
									fnCallBackDidAddType, fnCallBackDidRemoveType,
									fnCallBackDidInstanceReload, fnCallBackDidVarManagerReload,
									fnCallBackFatalError);

		InitHotReload();
	}
}
