//
//  Copyright (c) since 2014 - 2020 C++ Hot Reload SL. All rights reserved
//

#include "StdAfx.h"
#include "CppHotReloadConfig.h"
#include <AzFramework/IO/LocalFileIO.h>
#include <AzCore/std/string/string.h>
//
// C/C++
//
#include <regex>
#include <string>
//
// zlib for crc32 calculation like the build system
// 
#include <zlib.h>

namespace CppHotReload
{
	std::string ROOT_DIR;
	std::string ENGINE_DIR;
	std::string DEV_DIR;
	std::string BOOTSTRAP_FILENAME;

	std::string GAME_NAME;
	std::string GAME_DIR;
	std::vector<std::string> GAME_DIRS;
	std::string GAME_PCH_FILENAME;
	std::string GAME_PCH_OBJ_FILENAME;
	std::string BIN_TEMP_DIR;
	std::string THIRDPARTY_DIR;
	std::string GEMS_DIR;
	std::string FORCED_INCLUDE_FILES;
	std::string TARGET_UID;

	std::string CPP_HOT_RELOAD_DIR;
	std::string CPP_HOT_RELOAD_INTERMEDIATE_DIR;
	std::string CPP_HOT_RELOAD_SOURCE_DIR;
	std::string CPP_HOT_RELOAD_INCLUDE_FILES;
	std::string CPP_HOT_RELOAD_BINARIES_DIR;
	std::string CPP_HOT_RELOAD_CUSTOM_CONSTRUCTORS;
	std::string CPP_HOT_RELOAD_COMPILE_OPTIONS;
	std::string CPP_HOT_RELOAD_PREPROCESSOR_OPTIONS;
	std::string CPP_HOT_RELOAD_LINK_OPTIONS;
	std::string CPP_HOT_RELOAD_HOOK_FILENAME;

	uint32_t zlib_crc32(const std::string& string)
	{
		const uint8_t* bytes = reinterpret_cast<const uint8_t*>(string.c_str());
		const size_t   size  = string.size();

		uint32_t crc = crc32(0L, Z_NULL, 0);

		for (size_t i = 0; i < size; ++i)
		{
			crc = crc32(crc, bytes + i, 1);
		}

		return crc;
	}

	std::string py_build_system_crc32(const std::string& string)
	{
		const int MIN_UID_VALUE = 1024;
		const int MAX_UID_VALUE = 9999999;
		uint32_t zlib_crc = zlib_crc32(string);
		uint32_t py_build_system_crc32 = ((zlib_crc & 0xffffffff) + MIN_UID_VALUE) % MAX_UID_VALUE;
		return std::to_string(py_build_system_crc32);
	}

	const std::string& GetRootDir()							{ return ROOT_DIR; }
	const std::string& GetEngineDir()						{ return ENGINE_DIR; }
	const std::string& GetDevDir()							{ return DEV_DIR; }
	const std::string& GetBootstrapFileName()				{ return BOOTSTRAP_FILENAME; }

	const std::string& GetGameName()						{ return GAME_NAME; }
	const std::string& GetGameDir()							{ return GAME_DIR; }
	const std::vector<std::string>& GetGameDirs()			{ return GAME_DIRS; }
	const std::string& GetGamePchFileName()					{ return GAME_PCH_FILENAME; }
	const std::string& GetGamePchObjFileName()				{ return GAME_PCH_OBJ_FILENAME; }
	const std::string& GetBinTempDir()						{ return BIN_TEMP_DIR; }
	const std::string& GetThirdPartyDir()					{ return THIRDPARTY_DIR; }
	const std::string& GetGemsDir()							{ return GEMS_DIR; }
	const std::string& GetForcedIncludeFiles()				{ return FORCED_INCLUDE_FILES; }
	const std::string& GetTargetUid()						{ return TARGET_UID; }

	const std::string& GetCppHotReloadDir()					{ return CPP_HOT_RELOAD_DIR; }
	const std::string& GetCppHotReloadIntermediateDir()		{ return CPP_HOT_RELOAD_INTERMEDIATE_DIR; }
	const std::string& GetCppHotReloadSourceDir()			{ return CPP_HOT_RELOAD_SOURCE_DIR; }
	const std::string& GetCppHotReloadIncludeFiles()		{ return CPP_HOT_RELOAD_INCLUDE_FILES; }
	const std::string& GetCppHotReloadBinariesDir()			{ return CPP_HOT_RELOAD_BINARIES_DIR; }
	const std::string& GetCppHotReloadCustomConstructors()  { return CPP_HOT_RELOAD_CUSTOM_CONSTRUCTORS; }
	const std::string& GetCppHotReloadCompileOptions()		{ return CPP_HOT_RELOAD_COMPILE_OPTIONS; }
	const std::string& GetCppHotReloadPreprocessorOptions() { return CPP_HOT_RELOAD_PREPROCESSOR_OPTIONS; }
	const std::string& GetCppHotReloadLinkOptions()			{ return CPP_HOT_RELOAD_LINK_OPTIONS; }	 
	const std::string& GetCppHotReloadHookFileName()		{ return CPP_HOT_RELOAD_HOOK_FILENAME; }	 

	bool ResolveAndCheckPath(std::string& path)
	{
		AZ::IO::FileIOBase* fileIO = AZ::IO::FileIOBase::GetInstance();
		
		char resolvedPath[AZ_MAX_PATH_LEN] = {0};
		if (!fileIO->ResolvePath(path.c_str(), resolvedPath, AZ_MAX_PATH_LEN))
		{
			AZ_Error("C++ Hot Reload", false, "Invalid folder: %s\n", resolvedPath);
			return false;
		}

		if (fileIO->Exists(resolvedPath))
		{
			path = resolvedPath;
		}
		else
		{
			AZ_Error("C++ Hot Reload", false, "Folder doesn't exists: %s\n", resolvedPath);
			return false;
		}
		return true;
	}

	void LocateProjectPaths()
	{
		AZ::IO::FileIOBase* fileIO = AZ::IO::FileIOBase::GetInstance();
		AZ::IO::FileIOBase* fileSystem = AZ::IO::FileIOBase::GetDirectInstance();

		ROOT_DIR			= fileIO->GetAlias("@root@");
		ENGINE_DIR			= fileIO->GetAlias("@engroot@");
		DEV_DIR				= fileIO->GetAlias("@devroot@");
		BOOTSTRAP_FILENAME	= DEV_DIR + "/bootstrap.cfg";
		//
		// 3rdParty directory
		//
		THIRDPARTY_DIR = DEV_DIR + "/../3rdParty";
		if (!ResolveAndCheckPath(THIRDPARTY_DIR))
			return;
		//
		// Gems directory
		//
		GEMS_DIR = DEV_DIR + "/Gems";
		if (!ResolveAndCheckPath(GEMS_DIR))
			return;
		//
		// C++ Hot Reload 
		//
		CPP_HOT_RELOAD_DIR					= GEMS_DIR + "/CppHotReload";
		CPP_HOT_RELOAD_SOURCE_DIR			= CPP_HOT_RELOAD_DIR + "/Code/Source";
		CPP_HOT_RELOAD_BINARIES_DIR			= CPP_HOT_RELOAD_DIR + "/External/" + CPP_HOT_RELOAD_VERSION;
		CPP_HOT_RELOAD_INTERMEDIATE_DIR		= CPP_HOT_RELOAD_BINARIES_DIR + "/Intermediate";
		CPP_HOT_RELOAD_INCLUDE_FILES		= CPP_HOT_RELOAD_SOURCE_DIR + "/IncludeDirectories.txt";
		CPP_HOT_RELOAD_CUSTOM_CONSTRUCTORS	= CPP_HOT_RELOAD_SOURCE_DIR + "/CppHotReloadCustomContructors.h";
		CPP_HOT_RELOAD_HOOK_FILENAME		= CPP_HOT_RELOAD_SOURCE_DIR + "/CppHotReloadLyHook.h";

		if (!fileIO->Exists(CPP_HOT_RELOAD_DIR.c_str()) 
		&&  !fileIO->Exists(CPP_HOT_RELOAD_INTERMEDIATE_DIR.c_str()) 
		&&  !fileIO->Exists(CPP_HOT_RELOAD_SOURCE_DIR.c_str()) 
		&&  !fileIO->Exists(CPP_HOT_RELOAD_INCLUDE_FILES.c_str()) 
		&&  !fileIO->Exists(CPP_HOT_RELOAD_BINARIES_DIR.c_str()) 
		&&  !fileIO->Exists(CPP_HOT_RELOAD_CUSTOM_CONSTRUCTORS.c_str())
		&&  !fileIO->Exists(CPP_HOT_RELOAD_HOOK_FILENAME.c_str()))
		{
			AZ_Error("C++ Hot Reload", false, "C++ Hot Reload files not found:\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n", CPP_HOT_RELOAD_DIR, CPP_HOT_RELOAD_INTERMEDIATE_DIR, CPP_HOT_RELOAD_SOURCE_DIR, CPP_HOT_RELOAD_INCLUDE_FILES, CPP_HOT_RELOAD_BINARIES_DIR, CPP_HOT_RELOAD_CUSTOM_CONSTRUCTORS, CPP_HOT_RELOAD_HOOK_FILENAME);
			return;
		}
		//
		// Get the default project name from boostrap.cfg
		//
		if (fileIO->Exists(BOOTSTRAP_FILENAME.c_str()))
		{
			AZ::IO::HandleType file = AZ::IO::InvalidHandle;
			if (fileIO->Open(BOOTSTRAP_FILENAME.c_str(), AZ::IO::OpenMode::ModeRead | AZ::IO::OpenMode::ModeText, file) != AZ::IO::ResultCode::Success)
			{
				AZ_Error("C++ Hot Reload", false, "Impossible to find bootstrap.cfg file in: %s\n", BOOTSTRAP_FILENAME.c_str());
				return;
			}

			AZ::u64 actualSize = 0;
			fileIO->Size(file, actualSize);

			if (actualSize == 0)
			{
				fileIO->Close(file);
				AZ_Error("C++ Hot Reload", false, "bootstrap.cfg is empty [%s]\n", BOOTSTRAP_FILENAME.c_str());
				return;
			}

			AZStd::string fileContents;
			fileContents.resize_no_construct(actualSize + 1);

			// Reading the entire bootstrap fileBinT
			if (fileIO->Read(file, fileContents.data(), actualSize) != AZ::IO::ResultCode::Success)
			{
				AZ_Error("C++ Hot Reload", false, "Something went wrong reading bootstrap.cfg [%s]\n", BOOTSTRAP_FILENAME.c_str());
				return;
			}
			fileContents[actualSize] = 0;

			fileIO->Close(file);
			//
			// Project name
			//
			std::smatch matches;
			std::string content = fileContents.c_str();
			if (std::regex_search(content, matches, std::regex(R"((sys_game_folder=)(\w+))")))
			{
				GAME_NAME = matches[2];
			}
			else
			{
				AZ_Error("C++ Hot Reload", false, "Couldn't find sys_game_folder in bootstrap.cfg [%s]\n", BOOTSTRAP_FILENAME.c_str());
				return;
			}
		}
		//
		// Game path
		//
		GAME_DIR = DEV_DIR + "/" + GAME_NAME;
		if (!ResolveAndCheckPath(GAME_DIR))
			return;
		//
		// Game directories
		//
		{
			std::vector<std::string> gameDirs;

			std::function<bool(const char*)> searchGameDirsFunction;
			searchGameDirsFunction = [&fileSystem, &gameDirs, &searchGameDirsFunction](const char* filePath) -> bool
			{
				if (fileSystem->IsDirectory(filePath))
				{
					std::string dir = filePath;
					if (ResolveAndCheckPath(dir))
					{
						gameDirs.emplace_back(dir);
						AZ::IO::Result res = fileSystem->FindFiles(filePath, "*", searchGameDirsFunction);
					}
				}
				return true;
			};
			AZ::IO::Result res = fileSystem->FindFiles(GAME_DIR.c_str(), "*", searchGameDirsFunction);
			GAME_DIRS = std::move(gameDirs);
			//
			// Hot reload other components located in another folder
			//
			GAME_DIRS.emplace_back(CPP_HOT_RELOAD_SOURCE_DIR + "/Test");
		}
		//
		// Platform specific
		//
#if CPP_HOT_RELOAD_PLATFORM_WINDOWS
		//
		// Find PCH and OBJ of the PCH
		//
		{
			BIN_TEMP_DIR = DEV_DIR + "/BinTemp/" + LUMBERYARD_BIN_CONFIG_DIR;
			if (!ResolveAndCheckPath(BIN_TEMP_DIR))
				return;
			std::string foundPch;
			std::string foundPchObj;

			std::function<bool(const char*)> searchPchAndPchObjFunction;
			searchPchAndPchObjFunction = [&fileSystem, &foundPch, &foundPchObj, &searchPchAndPchObjFunction](const char* filePath) -> bool
			{
				if (fileSystem->IsDirectory(filePath))
				{
					AZ::IO::Result res = fileSystem->FindFiles(filePath, "*", searchPchAndPchObjFunction);
				}
				else
				{
					std::string filePathString = filePath;
					if (filePathString.find(LUMBERYARD_PCH_NAME) != std::string::npos)
					{
						char resolvedPath[AZ_MAX_PATH_LEN] = { 0 };
						if (fileSystem->ResolvePath(filePath, resolvedPath, AZ_MAX_PATH_LEN))
						{
							std::string fileName = resolvedPath;
							if (fileName.find(".pch") != std::string::npos)
							{
								foundPch = std::move(fileName);
							}
							else
							{
								foundPchObj = std::move(fileName);
							}
						}
					}
				}
				return true;
			};
			AZ::IO::Result res = fileSystem->FindFiles((BIN_TEMP_DIR + "/" + GAME_NAME).c_str(), "*", searchPchAndPchObjFunction);
			//
			// PCH and OBJ of the PCH
			//
			GAME_PCH_FILENAME = std::move(foundPch);
			GAME_PCH_OBJ_FILENAME = std::move(foundPchObj);
			if (!ResolveAndCheckPath(GAME_PCH_FILENAME))
				return;
			if (!ResolveAndCheckPath(GAME_PCH_OBJ_FILENAME))
				return;
		}
		//
		// TARGET_UID
		//
		TARGET_UID = py_build_system_crc32(GAME_NAME);
		if (GAME_PCH_FILENAME.find(TARGET_UID) != std::string::npos)
		{
			AZ_Printf("C++ Hot Reload", "sys_game_folder=%s, crc: %s\n", GAME_NAME.c_str(), TARGET_UID.c_str());
		}
		else
		{
			AZ_Error("C++ Hot Reload", false, "CRC32 of project doesn't match [%s]\n", TARGET_UID.c_str());
		}

		const std::string& PCH_INCLUDE = GAME_PCH_INCLUDE_NAME;
		const std::string& PCH_OPTIONS = "/FI\"" + PCH_INCLUDE + "\" /Yu\"" + PCH_INCLUDE + "\" /Fp\"" + GAME_PCH_FILENAME + "\" /FI\"" + CPP_HOT_RELOAD_CUSTOM_CONSTRUCTORS + "\" ";
		//
		// Force to include custom constructor and the pch
		//
#if CPP_HOT_RELOAD_IS_DEBUG
		CPP_HOT_RELOAD_COMPILE_OPTIONS = PCH_OPTIONS + R"(/MDd /bigobj /nologo /W3 /WX- /MP /Gy /GF /Gm- /fp:fast /Zc:wchar_t /Zc:forScope /GR- /Gd /Zc:inline /wd4530 /wd4577 /wd4005 /wd4240 /FS /std:c++17 /Zc:__cplusplus /Od /Ob0 /Oy- /RTC1 )";
		CPP_HOT_RELOAD_PREPROCESSOR_OPTIONS  = R"(/D__LUMBERYARD__ /DPRODUCT_SKU_default /D_WIN32 /D_WIN64 /D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING /D_ENABLE_EXTENDED_ALIGNED_STORAGE /D_PROFILE /DPROFILE /DNDEBUG /DAZ_ENABLE_TRACING /DAZ_ENABLE_DEBUG_TOOLS /DSCRIPTCANVAS_ERRORS_ENABLED /DEXE_VERSION_INFO_0=1 /DEXE_VERSION_INFO_1=21 /DEXE_VERSION_INFO_2=2 /DEXE_VERSION_INFO_3=0 /DPLATFORM_SUPPORTS_AWS_NATIVE_SDK /DUSE_WINDOWS_DLL_SEMANTICS /D_DLL /DLY_METRICS_BUILD_TIME=0 /DUSE_ZSTD )";
#else
		CPP_HOT_RELOAD_COMPILE_OPTIONS = PCH_OPTIONS = R"(/MD /bigobj /nologo /W3 /WX- /MP /Gy /GF /Gm- /fp:fast /Zc:wchar_t /Zc:forScope /GR- /Gd /Zc:inline /wd4530 /wd4577 /FS /std:c++17 /Zc:__cplusplus /Ox /Ob2 /Ot /Oi /Oy- )";
		CPP_HOT_RELOAD_PREPROCESSOR_OPTIONS  = R"(/D__LUMBERYARD__ /DPRODUCT_SKU_default /D_WIN32 /D_WIN64 /D_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING /D_ENABLE_EXTENDED_ALIGNED_STORAGE /D_PROFILE /DPROFILE /DNDEBUG /DAZ_ENABLE_TRACING /DAZ_ENABLE_DEBUG_TOOLS /DSCRIPTCANVAS_ERRORS_ENABLED /DEXE_VERSION_INFO_0=1 /DEXE_VERSION_INFO_1=21 /DEXE_VERSION_INFO_2=2 /DEXE_VERSION_INFO_3=0 /DPLATFORM_SUPPORTS_AWS_NATIVE_SDK /DUSE_WINDOWS_DLL_SEMANTICS /D_MT /D_DLL /DLY_METRICS_BUILD_TIME=0 /DUSE_ZSTD )";
#endif
		CPP_HOT_RELOAD_LINK_OPTIONS = "\"" + GAME_PCH_OBJ_FILENAME + "\" /NOLOGO /MANIFEST /LARGEADDRESSAWARE /INCREMENTAL:NO /MACHINE:X64 /OPT:REF /OPT:ICF /DEBUG ";
#endif
	}
}
