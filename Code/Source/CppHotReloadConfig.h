//
//  Copyright (c) since 2014 - 2020 C++ Hot Reload SL. All rights reserved
//

#pragma once
#define CPP_HOT_RELOAD_VERSION "0.9.98"
//
// Platform
//
#ifdef _MSC_VER
#	define CPP_HOT_RELOAD_PLATFORM_WINDOWS 1
#	define CPP_HOT_RELOAD_PLATFORM_MAC 0
#   if (_MSC_VER >= 1920)	// VS 2019
#		define CPP_HOT_RELOAD_VS_VAR_ALL "\"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat\""
#   elif (_MSC_VER > 1910)	// VS 2017
#		define CPP_HOT_RELOAD_VS_VAR_ALL "\"C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat\""
#	endif
#else
#	define CPP_HOT_RELOAD_PLATFORM_WINDOWS 0
#	define CPP_HOT_RELOAD_PLATFORM_MAC 1
#endif
#if CPP_HOT_RELOAD_PLATFORM_WINDOWS
#	define CPP_HOT_RELOAD_PLATFORM_NAME "Windows"
#	if _WIN64
#		define CPP_HOT_RELOAD_ARCH_NAME "x64"
#	else
#		error "Platform not supported"
#	endif
#elif CPP_HOT_RELOAD_PLATFORM_MAC
#		define CPP_HOT_RELOAD_ARCH_NAME "x86_64"
#endif
//
// Build configuration
//
#if _MSC_VER >= 1920
#	define VS_VERSION_SUPPORT "2019"
#elif _MSC_VER > 1910
#	define VS_VERSION_SUPPORT "2017"
#else
#	define VS_VERSION_SUPPORT "NOT_SUPPORTED"
#endif
#ifdef _DEBUG
#	define CPP_HOT_RELOAD_CONFIGURATION "Debug"
#	define CPP_HOT_RELOAD_IS_DEBUG 1
#	define LUMBERYARD_BIN_CONFIG_DIR	"win_x64_vs" VS_VERSION_SUPPORT "_debug"
#else
#	define CPP_HOT_RELOAD_CONFIGURATION "Release"
#	define CPP_HOT_RELOAD_IS_DEBUG 0
#	define LUMBERYARD_BIN_CONFIG_DIR	"win_x64_vs" VS_VERSION_SUPPORT "_profile"
#endif

namespace CppHotReload
{
	void LocateProjectPaths();
	enum class LyVersion
	{
		Unknown,
		LY_1_22,
		LY_1_23,
		LY_1_24
	};
	std::string GetFileContent(const std::string& path);
	LyVersion GetLumberyardVersion();
	std::string GetAdditionalLibrarPaths(LyVersion lyVersion);
	std::string GetAdditionalLibraries(LyVersion lyVersion);

	const std::string& GetRootDir();
	const std::string& GetEngineDir();
	const std::string& GetDevDir();
	const std::string& GetBootstrapFileName();

	const std::string& GetGameName();
	const std::string& GetGameDir();	
	const std::vector<std::string>& GetGameDirs();			
	const std::string& GetGamePchFileName();
	const std::string& GetGamePchObjFileName(); 
	const std::string& GetBinTempDir();			
	const std::string& GetThirdPartyDir();		
	const std::string& GetGemsDir();			
	const std::string& GetForcedIncludeFiles(); 
	const std::string& GetTargetUid();			

	const std::string& GetCppHotReloadDir();		
	const std::string& GetCppHotReloadIntermediateDir();
	const std::string& GetCppHotReloadSourceDir();			 
	const std::string& GetCppHotReloadIncludeFiles();		 
	const std::string& GetCppHotReloadBinariesDir();		 
	const std::string& GetCppHotReloadCustomConstructors();  
	const std::string& GetCppHotReloadCompileOptions();		 
	const std::string& GetCppHotReloadPreprocessorOptions(); 
	const std::string& GetCppHotReloadLinkOptions();		 
	const std::string& GetCppHotReloadHookFileName();		 
}
