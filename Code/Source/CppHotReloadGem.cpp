//
//  Copyright (c) since 2014 - 2020 C++ Hot Reload SL. All rights reserved
//

#include "StdAfx.h"
#include <IGem.h>
#include <CryLibrary.h>
//
// Ly
//
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>
#include <AzCore/Component/EntityBus.h>
#include <AzCore/Component/Entity.h>
#include <AzCore/std/string/conversions.h>
#include <AzToolsFramework/ToolsComponents/GenericComponentWrapper.h>
//
// C++ Hot Reload
//
#include "CppHotReload/HotReloadAPI.h"
#include "CppHotReloadInit.h"
#include "CppHotReloadEvents.h"
#include "CppHotReloadSubscriber.h"
//
// Test
//
#include "Test/CppHotReloadTestComponent.h"
//
// C++ Hot Reload - logs
//
namespace
{
    void LogFatal(const char* msg)
    {
        AZ_Error("C++ Hot Reload", false, "%s\n", msg);
    }
    void LogCritical(const char* msg)
    {
        AZ_Error("C++ Hot Reload", false, "%s\n", msg);
    }
    void LogError(const char* msg)
    {
        AZ_Error("C++ Hot Reload", false, "%s\n", msg);
    }
    void LogWarning(const char* msg)
    {
        AZ_Warning("C++ Hot Reload", false, "%s\n", msg);
    }
    void LogSystem(const char* msg)
    {
        AZ_Printf("C++ Hot Reload", "%s\n", msg);
    }
}
//
// C++ Hot Reload - Module
//
namespace CppHotReload
{
    class CppHotReloadModule
        : public CryHooksModule
        , public AZ::EntitySystemBus::Handler
        , public AZ::SystemTickBus::Handler
    {
    private:
        /* Editor purpose, not yet!
        enum class CppHotReloadStatus
        {
            WAITING = 0,
            UNREAL_HEADER_TOOL,
            BUILDING
        };
        CppHotReloadStatus cppHotReloadStatus;
        */
        void* m_ppHotReloadHandle;

    public:
        AZ_RTTI(CppHotReloadModule, "{9462F100-77B6-4096-A259-4A1A8D4A7C82}", AZ::Module);
        AZ_CLASS_ALLOCATOR(CppHotReloadModule, AZ::SystemAllocator, 0);

        CppHotReloadModule()
            : CryHooksModule()
            , m_ppHotReloadHandle(nullptr)
        {
            m_descriptors.insert(m_descriptors.end(), 
            {
                CppHotReloadTestComponent::CreateDescriptor(),
            });
        }

        void OnSystemTick() override
        {
            //
            // Noete: during prototype face execute this on the background while the dlls are not created will make lose the ability to place breakpoints
            // So, I only update C++ Hot Reload if we are going to reload something...
            //
            if (!CppHotReload::IsWorking())
            {
                CppHotReload::Update();
                CppHotReload::Purge();
            }
        }
        //
        // C++ Hot Reload is currently configured in the prototype during play mode
        //
        void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override
        {
            switch (event)
            {
            case ESYSTEM_EVENT_LEVEL_LOAD_START:
            {
                AZ::EntitySystemBus::Handler::BusConnect();
                AZ::SystemTickBus::Handler::BusConnect();
            }
            break;
            case ESYSTEM_EVENT_LEVEL_POST_UNLOAD:
            {
                AZ::EntitySystemBus::Handler::BusDisconnect();
                AZ::SystemTickBus::Handler::BusDisconnect();
            }
            break;
            case ESYSTEM_EVENT_GAME_POST_INIT:
            {
                m_ppHotReloadHandle = CryLoadLibrary(CPPHOTRELOAD_DLL);
                if (m_ppHotReloadHandle)
                {
                    CppHotReload::Init(CppHotReload::BeginReload, CppHotReload::EndReload, CppHotReload::FilterFileToBuild, CppHotReload::WillCompileFile, CppHotReload::DidCompileFile, CppHotReload::WillLinkFile, CppHotReload::DidLinkFile, CppHotReload::DidAddType, CppHotReload::DidRemoveType, CppHotReload::DidReloadInstance, CppHotReload::DidReloadVarManager, CppHotReload::DidFatalErrorScript, LogFatal, LogCritical, LogError, LogWarning, LogSystem);

                    if (!CppHotReload::IsInitialized())
                    {
                        LogFatal("Something went wrong initializing C++ HotReload!\n");
                    }
                }
                else
                {
                    LogFatal("C++ HotReload library not loaded!\n");
                }
            }
            break;
            case ESYSTEM_EVENT_GAME_MODE_SWITCH_START:
                break;
            case ESYSTEM_EVENT_GAME_MODE_SWITCH_END:
                break;
            case ESYSTEM_EVENT_EDITOR_GAME_MODE_CHANGED:
            case ESYSTEM_EVENT_EDITOR_SIMULATION_MODE_CHANGED:
            {
                if (gEnv->IsEditor())
                {
                    if (wparam == 0)
                    {

                    }
                    else
                    {

                    }
                }
            }
            break;
            case ESYSTEM_EVENT_FAST_SHUTDOWN:
            case ESYSTEM_EVENT_FULL_SHUTDOWN:
            {
                if (m_ppHotReloadHandle)
                {
                    CryFreeLibrary(m_ppHotReloadHandle);
                }
            }
            break;
            }
        }
        //
        // Note: Entities and components are "created and deleted" after play/stop the scene, as I don't know yet
        // where is the original pointer, C++ Hot Reload is only activated wehn Activated/Deactivated, which happens
        // during play/stop mode. Basically will only reload when is in play mode
        //
        void OnEntityInitialized(const AZ::EntityId& entityId) override
        {
            const AZStd::string& msg = entityId.ToString() + " OnEntityInitialized";
            LogSystem(msg.c_str());
            RegisterEntityComponents(entityId);
        }
        void OnEntityDestroyed(const AZ::EntityId& entityId) override
        {
            const AZStd::string& msg = entityId.ToString() + " OnEntityDestroyed";
            LogSystem(msg.c_str());
            UnregisterEntityComponents(entityId);
        }
        void OnEntityActivated(const AZ::EntityId& entityId) override
        {
        }
        void OnEntityDeactivated(const AZ::EntityId& entityId) override
        {
        }
        //void OnEntityNameChanged(const AZ::EntityId& entityId, const AZStd::string& name) override;

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
                    CppHotReload::ComponentType type = CppHotReload::ComponentType::INSTANCE;

                    if (auto genericComponentWrapper = azrtti_cast<AzToolsFramework::Components::GenericComponentWrapper*>(currComponent))
                    {
                        type = CppHotReload::ComponentType::GENERIC;
                        currComponent = genericComponentWrapper->GetTemplate();
                    }
                    else if (auto genericComponentEditor = azrtti_cast<AzToolsFramework::Components::EditorComponentBase*>(currComponent))
                    {
                        type = CppHotReload::ComponentType::EDITOR;
                    }

                    const AZStd::string& componentId = AZStd::to_string(currComponent->GetId());  
                    const AZStd::string& typeName = currComponent->RTTI_GetTypeName();
                    const AZStd::string& guid = CppHotReload::RegisterPtr(typeName.c_str(), reinterpret_cast<void*>(currComponent));
                    CppHotReload::SubscribeToHotReload(CppHotReload::HotReloadPtr { guid, entity, currComponent, type });

                    AZ_Printf("C++ Hot Reload", false, "Component with id: %s registered for C++ Hot Reload\n", guid.c_str());
                }
            }
        }

        void UnregisterEntityComponents(const AZ::EntityId& entityId)
        {
            AZ::Entity* entity = GetEntityById(entityId);
            if (entity != nullptr)
            {
                auto components = entity->GetComponents();  

                for (AZ::Component* currComponent : components)      
                {  
                    CppHotReload::ComponentType type = CppHotReload::ComponentType::INSTANCE;

                    if (auto genericComponentWrapper = azrtti_cast<AzToolsFramework::Components::GenericComponentWrapper*>(currComponent))
                    {
                        type = CppHotReload::ComponentType::GENERIC;
                        currComponent = genericComponentWrapper->GetTemplate();
                    }
                    else if (auto genericComponentEditor = azrtti_cast<AzToolsFramework::Components::EditorComponentBase*>(currComponent))
                    {
                        type = CppHotReload::ComponentType::EDITOR;
                    }

                    const AZStd::string& uuid = CppHotReload::GetGuidFromSubscriber(currComponent);
                    const AZStd::string& typeName = currComponent->RTTI_GetTypeName();
                    CppHotReload::UnregisterPtr(typeName.c_str(), reinterpret_cast<void*>(currComponent), uuid.c_str());
                    CppHotReload::UnsubscribeToHotReload(currComponent, type);

                    AZ_Printf("C++ Hot Reload", false, "Component template with id: %s unregistered for C++ Hot Reload\n", uuid.c_str());
                }
            }
        }
    };
}

// DO NOT MODIFY THIS LINE UNLESS YOU RENAME THE GEM
// The first parameter should be GemName_GemIdLower
// The second should be the fully qualified name of the class above
AZ_DECLARE_MODULE_CLASS(CppHotReload_29f2b034d17d42cca8a3f3cfe05a706f, CppHotReload::CppHotReloadModule)
