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
	std::string CPP_HOT_RELOAD_PATH;
	std::string GAME_NAME;
	std::string GAME_DIR;
	std::string GAME_PCH_FILENAME;
	std::string GAME_PCH_OBJ_FILENAME;
	std::string BIN_TEMP_DIR;
	std::string THIRDPARTY_DIR;
	std::string FORCED_INCLUDE_FILES;
	std::string TARGET_UID;

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

	void LocateProjectPaths()
	{
		AZ::IO::FileIOBase* fileIO = AZ::IO::FileIOBase::GetInstance();
		const std::string& rootPath = fileIO->GetAlias("@root@");
		const std::string& engrootPath = fileIO->GetAlias("@engroot@");
		const std::string& devrootPath = fileIO->GetAlias("@devroot@");
		const std::string& bootstrapFilename = devrootPath + "/bootstrap.cfg";
		//
		// 3rdParty directory
		//
		THIRDPARTY_DIR = rootPath + "/../3rdParty";
		//
		// Get the default project name from boostrap.cfg
		//
		if (fileIO->Exists(bootstrapFilename.c_str()))
		{
			AZ::IO::HandleType file = AZ::IO::InvalidHandle;
			if (fileIO->Open(bootstrapFilename.c_str(), AZ::IO::OpenMode::ModeRead | AZ::IO::OpenMode::ModeText, file) != AZ::IO::ResultCode::Success)
			{
				AZ_Error("C++ Hot Reload", false, "Impossible to find bootstrap.cfg file in: %s\n", bootstrapFilename.c_str());
				return;
			}

			AZ::u64 actualSize = 0;
			fileIO->Size(file, actualSize);

			if (actualSize == 0)
			{
				fileIO->Close(file);
				AZ_Error("C++ Hot Reload", false, "bootstrap.cfg is empty [%s]\n", bootstrapFilename.c_str());
				return;
			}

			AZStd::string fileContents;
			fileContents.resize_no_construct(actualSize + 1);

			// Reading the entire bootstrap fileBinT
			if (fileIO->Read(file, fileContents.data(), actualSize) != AZ::IO::ResultCode::Success)
			{
				AZ_Error("C++ Hot Reload", false, "Something went wrong reading bootstrap.cfg [%s]\n", bootstrapFilename.c_str());
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
				AZ_Error("C++ Hot Reload", false, "Couldn't find sys_game_folder in bootstrap.cfg [%s]\n", bootstrapFilename.c_str());
				return;
			}
		}
		//
		// Game path
		//
		GAME_DIR = devrootPath + "/" + GAME_NAME;

#if CPP_HOT_RELOAD_PLATFORM_WINDOWS
		//
		// FInd PCH and OBJ of the PCH
		//
		BIN_TEMP_DIR = devrootPath + "/BinTemp/" + LUMBERYARD_BIN_CONFIG_DIR + GAME_NAME;

		AZ::IO::FileIOBase* fileSystem = AZ::IO::FileIOBase::GetDirectInstance();
		std::string foundPch;
		std::string foundPchObj;

		std::function<bool(const char*)> searchFunction;
		searchFunction = [&fileSystem, &foundPch, &foundPchObj, &searchFunction](const char* filePath) -> bool
		{
			if (fileSystem->IsDirectory(filePath))
			{
				AZ::IO::Result res = fileSystem->FindFiles(filePath, "*", searchFunction);
			}
			else
			{
				std::string filePathString = filePath;
				if (filePathString.find(LUMBERYARD_PCH_NAME) != std::string::npos)
				{
					char resolvedPath[AZ_MAX_PATH_LEN] = {0};
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
		AZ::IO::Result res = fileSystem->FindFiles(BIN_TEMP_DIR.c_str(), "*", searchFunction);
		//
		// TARGET_UID
		//
		TARGET_UID = py_build_system_crc32(GAME_NAME);
		AZ_Printf("C++ Hot Reload", "sys_game_folder=%s, crc: %s\n", GAME_NAME, TARGET_UID.c_str());
		//
		// PCH
		//
		GAME_PCH_FILENAME = std::move(foundPch);
		GAME_PCH_OBJ_FILENAME = std::move(foundPchObj);
#endif
	}
}
