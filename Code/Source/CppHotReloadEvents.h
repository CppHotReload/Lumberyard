//
//  Copyright (c) 2014 - 2019 C++ Hot Reload SL. All rights reserved
//
//  This file is subject to the terms and conditions defined in
//  file 'LICENSE.txt', which is part of this source code package.
//

#pragma once
//
// C++ Hot Reload
//
#include "CppHotReload/HotReloadAPI.h"

namespace CppHotReload
{
	void HotReload();
	void BeginReload(const char* const fileThatTriggeredReload, const char*& commandToExecuteBeforeReload);
	void EndReload();
	void FilterFileToBuild(const char* const filePath, bool& hasToBuild);
	void WillCompileFile(const char* const filePath, const char*& additionalFilesToCompile, bool& addAdditionalFilesToMainLibrary, const char*& addAdditionalOptionsToCompile);
	void DidCompileFile(const char* const filePath, bool wasOk);
	void WillLinkFile(const char* const filePath, const char*& additionalObjsToLink);
	void DidLinkFile(VarManager* vmFile, bool wasOk);
	void DidAddType(TypeFileInfo* scriptFile);
	void DidRemoveType(TypeFileInfo* scriptFile);
	void DidReloadInstance(const char* const typeName, void*& data, const char* uuid);
	void DidReloadVarManager(VarManager* varManager, void*& data);
	void DidFatalErrorScript(const char* const info);
}
