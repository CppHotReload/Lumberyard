//
//  Copyright (c) since 2014 - 2020 C++ Hot Reload SL. All rights reserved
//

#pragma once
//
// C++ Hot Reload
//
#include "CppHotReload/HotReloadAPI.h"
#include <AzCore/Component/Component.h>

namespace CppHotReload
{
	void ParsingFileProgress(const size_t currentNumFile, const size_t totalNumFiles, const char* const fileName);
	void BeginReload(const char* const fileThatTriggeredReload, const char*& commandToExecuteBeforeReload);
	void EndReload();
	void FilterFileToBuild(const char* const filePath, bool& hasToBuild);
	void WillCompileFile(const char* const filePath, const char*& additionalFilesToCompile);
	void DidCompileFile(const char* const filePath, bool wasOk);
	void WillLinkFile(const char* const filePath, const char*& additionalObjsToLink);
	void DidLinkFile(VarManager* vmFile, bool wasOk);
	void DidAddType(TypeFileInfo* scriptFile);
	void DidRemoveType(TypeFileInfo* scriptFile);
	void DidReloadInstance(const char* const typeName, void*& data, const char* uuid);
	void DidReloadVarManager(VarManager* varManager, void*& data);
	void DidFatalErrorScript(const char* const info);

    void RegisterComponent(AZ::Entity* entity, AZ::Component* component);
    void UnregisterAliveComponent(AZ::Entity* entity, AZ::Component* component);

	void RegisterEntityComponents(const AZ::EntityId& entityId);
    void UnregisterEntityComponents(const AZ::EntityId& entityId);
}
