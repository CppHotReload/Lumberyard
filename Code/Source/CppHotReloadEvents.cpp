//
//  Copyright (c) 2014 - 2019 C++ Hot Reload SL. All rights reserved
//
//  This file is subject to the terms and conditions defined in
//  file 'LICENSE.txt', which is part of this source code package.
//

#include "StdAfx.h"
#include "CppHotReloadEvents.h"
#if defined(CPP_HOT_RELOAD_API)
//
// C/C++ Hot Reload
//
#include "CppHotReload/Serialization.h"
//
// Gem
//
#include "CppHotReloadConfig.h"
#include "CppHotReloadInit.h"
#include "CppHotReloadSubscriber.h"
//
// C/C++
//
#include <iostream>
#include <atomic>
//
// Ly
//
#include <AzCore/Component/Entity.h>
#include <AzCore/Component/Component.h>
//
// Illustrates all C++ Hot Reload events, just logging information
//
namespace CppHotReload
{	
	//
	// BeginReload and EngReload are triggered in 2 phases:
	// 1.- When in the background files are processed, new libraries are build, loaded
	// 2.- When from the game engine it's trigger to Update and Purge C++ Hot Reload
	// Note: if fileThatTriggeredReload and commandToExecuteBeforeReload are nullptr are in purging step
	//
	void BeginReload(const char* const fileThatTriggeredReload, const char*& commandToExecuteBeforeReload)
	{
		if (fileThatTriggeredReload != nullptr)
		{
			// Any header tool?
		}
	}

	void EndReload()
	{
		
	}

	void FilterFileToBuild(const char* const filePath, bool& hasToBuild)
	{
		//
		// Reload all 
		//
		hasToBuild = true;
	}

	void WillCompileFile(const char* const filePath, const char*& additionalFilesToCompile, bool& addAdditionalFilesToMainLibrary, const char*& addAdditionalOptionsToCompile)
	{
		AZ_Printf("C++ Hot Reload", "WillCompileFile: %s\n", filePath);
	}

	void DidCompileFile(const char* const filePath, bool wasOk)
	{
		if (wasOk) AZ_Printf("C++ Hot Reload", "DidCompileFile: %s compiled successful\n", filePath)
		else	   AZ_Printf("C++ Hot Reload", "DidCompileFile: Failed to compile: %s\n", filePath)
	}

	void WillLinkFile(const char* const filePath, const char*& additionalObjsToLink)
	{
		AZ_Printf("C++ Hot Reload", "Linking: %s\n", filePath);
	}

	void DidLinkFile(VarManager* vmFile, bool wasOk)
	{
		if (wasOk)	AZ_Printf("C++ Hot Reload", "%s linked successful\n", vmFile->keyName.c_str())
		else		AZ_Printf("C++ Hot Reload", "Failed to link the type %s\n", vmFile->keyName.c_str())
	}

	void DidAddType(TypeFileInfo* scriptFile)
	{
		AZ_Printf("C++ Hot Reload", "DidAddType: New type added: %s\n", scriptFile->typeName);
	}

	void DidRemoveType(TypeFileInfo* scriptFile)
	{
		AZ_Printf("C++ Hot Reload", "DidRemoveType: Type %s removed\n", scriptFile->typeName);
	}

	void DidReloadInstance(const char* const typeName, void*& data, const char* uuid)
	{
		HotReloadPtr subscriber;
		subscriber.component = nullptr; 
		//
		// Search component by instance
		//
		for (const HotReloadPtr& currSubscriber : hotReloadSubscribers)
		{
			if (currSubscriber.guid == uuid)
			{
				subscriber = currSubscriber;
				break;
			}
		}

		if (subscriber.component)
		{
			auto newComponent = reinterpret_cast<AZ::Component*>(data);
			//
			// Swap components for the active components. Normally because the user is in play mode
			//
			if (subscriber.component->GetEntity())
			{
				if (subscriber.entity)
				{
					subscriber.entity->Deactivate();
					subscriber.entity->SwapComponents(subscriber.component, newComponent);
					subscriber.entity->Activate();
					AZ_Printf("C++ Hot Reload", "DidReloadInstance: Type %s with id %s reloaded\n", typeName, uuid);
				}
			}
			else
			{
				subscriber.component = newComponent;
			}
			//UpdateHotReloadSubscriber(HotReloadPtr{ uuid, subscriber.entity, newComponent });
		}
	}

	void DidReloadVarManager(VarManager* varManager, void*& data)
	{
		AZ_Printf("C++ Hot Reload", "DidReloadVarManager:\n%s\n", varManager->ToString().c_str());
	}

	void DidFatalErrorScript(const char* const info)
	{
		std::cerr << "Fatal error!!";
		AZ_Error("C++ Hot Reload", false, "DidFatalErrorScript:%s\n", info);
		//
		// This is a critical point. It's needed to call a secure restoration point.
		// In the case of a tool or a multi-threaded program the user must define where
		// is a secure point / method / memory address to restore
		//
		AZ_Printf("C++ Hot Reload", "Recovering previous state!");
		CppHotReload::Restore();
	}
}

#endif // CPP_HOT_RELOAD_API
