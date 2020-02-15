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
    {
    private:
        enum class CppHotReloadStatus
        {
            WAITING = 0,
            UNREAL_HEADER_TOOL,
            BUILDING
        };

        void* m_ppHotReloadHandle;
        bool hasToRegisterCppHotReloadPtr;
        CppHotReloadStatus cppHotReloadStatus;
        AZStd::vector<AZ::EntityId> entitiesToRegister;
        AZStd::vector<AZ::EntityId> entitiesToUnregister;

    public:
        AZ_RTTI(CppHotReloadModule, "{9462F100-77B6-4096-A259-4A1A8D4A7C82}", AZ::Module);
        AZ_CLASS_ALLOCATOR(CppHotReloadModule, AZ::SystemAllocator, 0);

        CppHotReloadModule()
            : CryHooksModule()
            , m_ppHotReloadHandle(nullptr)
            , hasToRegisterCppHotReloadPtr(false)
        {
            m_descriptors.insert(m_descriptors.end(), 
            {
                CppHotReloadTestComponent::CreateDescriptor(),
            });
        }
        
        void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override
        {
            switch (event)
            {
            case ESYSTEM_EVENT_LEVEL_LOAD_START:
            {
            }
            break;
            case ESYSTEM_EVENT_LEVEL_POST_UNLOAD:
            {
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
                AZ::EntitySystemBus::Handler::BusConnect();
            }
            break;
            case ESYSTEM_EVENT_GAME_MODE_SWITCH_START:
                if (wparam == 1)
                    hasToRegisterCppHotReloadPtr = true;
                else
                    hasToRegisterCppHotReloadPtr = false;
                break;

            case ESYSTEM_EVENT_GAME_MODE_SWITCH_END:
                if (wparam == 1)
                    RegisterComponentsToHotReload();
                else
                    UnregisterComponentsToHotReload();
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
                    CryFreeLibrary(m_ppHotReloadHandle);
                AZ::EntitySystemBus::Handler::BusDisconnect();
            }
            break;
            }
        }

        //////////////////////////////////////////////////////////////////////////
        // AZ::EntitySystemBus::Handler
        void OnEntityInitialized(const AZ::EntityId& entityId) override
        {
            const AZStd::string& msg = entityId.ToString() + " OnEntityInitialized";
            LogSystem(msg.c_str());
        }
        void OnEntityDestroyed(const AZ::EntityId& entityId) override
        {
            const AZStd::string& msg = entityId.ToString() + " OnEntityDestroyed";
            LogSystem(msg.c_str());
        }
        void OnEntityActivated(const AZ::EntityId& entityId) override
        {
            if (hasToRegisterCppHotReloadPtr)
                entitiesToRegister.emplace_back(entityId);
        }
        void OnEntityDeactivated(const AZ::EntityId& entityId) override
        {
            if (hasToRegisterCppHotReloadPtr)
                entitiesToUnregister.emplace_back(entityId);
        }
        //void OnEntityNameChanged(const AZ::EntityId& entityId, const AZStd::string& name) override;
        //////////////////////////////////////////////////////////////////////////

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

        void RegisterComponentsToHotReload()
        {
            for (const AZ::EntityId& entityId : entitiesToRegister)
            {
                AZ::Entity* entity = GetEntityById(entityId);
                if (entity == nullptr)
                    continue;
                auto components = entity->GetComponents();  

                for (AZ::Component* currComponent : components)      
                {  
                    if (!currComponent)
                        continue;
                    const AZStd::string& componentId = AZStd::to_string(currComponent->GetId());  
                    const AZStd::string& className = currComponent->RTTI_GetTypeName();
                    const AZStd::string& guid = CppHotReload::RegisterPtr(className.c_str(), reinterpret_cast<void*>(currComponent));
                    CppHotReload::SubscribeToHotReload(CppHotReload::HotReloadPtr { guid, entity, currComponent });
                    AZ_Warning("C++ Hot Reload", false, "Component with id: %s registered for C++ Hot Reload\n", guid.c_str());
                }
            }
            entitiesToRegister.clear();
        }

        void UnregisterComponentsToHotReload()
        {
            CppHotReload::hotReloadSubscribers.clear();
            for (const AZ::EntityId& entityId : entitiesToUnregister)
            {
                AZ::Entity* entity = GetEntityById(entityId);
                if (entity == nullptr)
                    continue;
                auto components = entity->GetComponents();  

                for (AZ::Component* currComponent : components)      
                {  
                    if (!currComponent)
                        continue;
                    const AZStd::string& componentId = AZStd::to_string(currComponent->GetId());  
                    const AZStd::string& className = currComponent->RTTI_GetTypeName();
                    CppHotReload::UnregisterPtr(className.c_str(), reinterpret_cast<void*>(currComponent), componentId.c_str());
                    //CppHotReload::UnsubscribeToHotReload(currComponent);
                }
            }
            entitiesToUnregister.clear();
        }

    };
}

// DO NOT MODIFY THIS LINE UNLESS YOU RENAME THE GEM
// The first parameter should be GemName_GemIdLower
// The second should be the fully qualified name of the class above
AZ_DECLARE_MODULE_CLASS(CppHotReload_29f2b034d17d42cca8a3f3cfe05a706f, CppHotReload::CppHotReloadModule)