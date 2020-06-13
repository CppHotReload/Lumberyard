//
//  Copyright (c) since 2014 - 2020 C++ Hot Reload SL. All rights reserved
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
#include <AzCore/Component/ComponentApplicationBus.h>
#include <AzToolsFramework/ToolsComponents/GenericComponentWrapper.h>
#include <AzCore/std/string/string.h>
#include "Test/CppHotReloadTestComponent.h"
//
// Illustrates all C++ Hot Reload events, just logging information
//
namespace CppHotReload
{
	void ParsingFileProgress(const size_t currentNumFile, const size_t totalNumFiles, const char* const fileName)
	{
		const auto& CurrentNumFile = std::to_string(currentNumFile);
		const auto& TotalNumFiles = std::to_string(totalNumFiles);

		AZ_Printf("C++ Hot Reload", "Processing File [%s/%s] %s\n", CurrentNumFile.c_str(), TotalNumFiles.c_str(), fileName);
	}
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

	void WillCompileFile(const char* const filePath, const char*& additionalFilesToCompile)
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
		//
		// Search component by instance
		//
		HotReloadPtr* subscriber;
		for (HotReloadPtr& currSubscriber : hotReloadSubscribers)
		{
			if (currSubscriber.guid == uuid)
			{
				subscriber = &currSubscriber;
				break;
			}
		}

		if (subscriber->component)
		{
			auto newComponent = reinterpret_cast<AZ::Component*>(data);
			//
			// Swap components for the active components. Normally because the user is in play mode
			//
			if (subscriber->entity)
			{
				//UnregisterAliveComponent(subscriber.entity, subscriber.component);
				AZ::Entity::State originalState = subscriber->entity->GetState();
				//
				// Deactivate the entity to be able to operate with it
				//
				if (originalState == AZ::Entity::ES_ACTIVE)
				{
					subscriber->entity->Deactivate();
				}
				else
				{
					AZ_Warning("C++ Hot Reload", false, "%s might need the user STOP/PLAY to see the reloaded code\n", typeName);
				}
				//
				// Swap Components unless is a GenericComponentWrapper that holds a template pointer
				//
				if (subscriber->type == CppHotReload::ComponentType::GENERIC)
				{
					//
					// Set template doesn't exists in Ly
					//
					//auto genericComponentWrapper = azrtti_cast<AzToolsFramework::Components::GenericComponentWrapper*>(subscriber->genericComponentWrapper);
					//genericComponentWrapper->SetTemplate(newComponent);
				}
				else
				{
					if (!subscriber->entity->RemoveComponent(subscriber->component))
					{
						AZ_Warning("C++ Hot Reload", false, "Couldn't remove component of type %s!\n", typeName);
					}
					
					if (!subscriber->entity->AddComponent(newComponent))
					{
						AZ_Warning("C++ Hot Reload", false, "Couldn't add component of type %s!\n", typeName);
					}
				}
				//
				// Store the new component for the next reload
				//
				subscriber->component = newComponent;
				subscriber->entity->EvaluateDependencies();
				//
				// Time to re-activate the entity
				//
				if (originalState == AZ::Entity::ES_ACTIVE)
				{
					subscriber->entity->Activate();
				}

				AZ_Printf("C++ Hot Reload", "DidReloadInstance: Type %s with id %s reloaded\n", typeName, uuid);
				//UpdateHotReloadSubscriber(HotReloadPtr{ uuid, subscriber.entity, newComponent });
			}
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

	void RegisterComponent(AZ::Entity* entity, AZ::Component* component)
	{
		CppHotReload::ComponentType type = CppHotReload::ComponentType::INSTANCE;
		AzToolsFramework::Components::GenericComponentWrapper* genericComponentWrapper = nullptr;

		if (auto ptr = azrtti_cast<AzToolsFramework::Components::GenericComponentWrapper*>(component))
		{
			type = CppHotReload::ComponentType::GENERIC;
			component = ptr->GetTemplate();
			genericComponentWrapper = ptr;
		}
		else if (auto ptr = azrtti_cast<AzToolsFramework::Components::EditorComponentBase*>(component))
		{
			type = CppHotReload::ComponentType::EDITOR;
		}

		const AZStd::string& typeName = component->RTTI_GetTypeName();
		const AZStd::string& entityName = entity->GetName();
		const AZStd::string& guid = CppHotReload::RegisterPtr(typeName.c_str(), reinterpret_cast<void*>(component));
		CppHotReload::SubscribeToHotReload(CppHotReload::HotReloadPtr { guid, typeName, entityName, entity->GetId(), entity, component, genericComponentWrapper, type });

		AZ_Printf("C++ Hot Reload", "RegisterComponent [%s]::[%s] - id: %s\n", entityName.c_str(), typeName.c_str(), guid.c_str());
	}

	void UnregisterAliveComponent(AZ::Entity* entity, AZ::Component* component)
	{
		CppHotReload::ComponentType type = CppHotReload::ComponentType::INSTANCE;

		if (auto genericComponentWrapper = azrtti_cast<AzToolsFramework::Components::GenericComponentWrapper*>(component))
		{
			type = CppHotReload::ComponentType::GENERIC;
			component = genericComponentWrapper->GetTemplate();
		}
		else if (auto genericComponentEditor = azrtti_cast<AzToolsFramework::Components::EditorComponentBase*>(component))
		{
			type = CppHotReload::ComponentType::EDITOR;
		}

		const AZStd::string& guid = CppHotReload::GetGuidFromSubscriber(component);
		const AZStd::string& typeName = component->RTTI_GetTypeName();
		const AZStd::string& entityName = entity->GetName();
		CppHotReload::UnregisterPtr(typeName.c_str(), reinterpret_cast<void*>(component), guid.c_str());
		CppHotReload::UnsubscribeToHotReload(component, type);

		AZ_Printf("C++ Hot Reload", "UnregisterAliveComponent [%s]::[%s] - id: %s\n", entityName.c_str(), typeName.c_str(), guid.c_str());
	}

	AZ::Entity* GetEntityById(const AZ::EntityId& entityId)
	{
		AZ_Assert(entityId.IsValid(), "Invalid EntityId provided.");
		if (!entityId.IsValid())
		{
			return nullptr;
		}

		AZ::Entity* entity = nullptr;
		EBUS_EVENT_RESULT(entity, AZ::ComponentApplicationBus, FindEntity, entityId);
		return entity;
	}

	void RegisterEntityComponents(const AZ::EntityId& entityId)
	{
		AZ::Entity* entity = GetEntityById(entityId);
		if (entity != nullptr)
		{
			auto components = entity->GetComponents();  

			for (AZ::Component* currComponent : components)      
			{  
				RegisterComponent(entity, currComponent);
			}
		}
	}

	void UnregisterEntityComponents(const AZ::EntityId& entityId)
	{
		UnregisterComponentsByEntityId(entityId);
	}
}

#endif // CPP_HOT_RELOAD_API
