//
//  Copyright (c) since 2014 - 2020 C++ Hot Reload SL. All rights reserved
//

#include "StdAfx.h"
#include <platform_impl.h>
#include <IGem.h>
#include <CryLibrary.h>
//
// C++ Hot Reload
//
#include <thread>
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
        std::thread thCppHotReload;
        std::atomic<bool> didFinish = false;
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
            
        }

        void UpdateHotReload()
        {
            while (!didFinish)
            {
                if (CppHotReload::HasToPurge())
                {
                    CppHotReload::Purge();
                    //CppHotReload::DeleteOldLibraries();   // Not yet
                }
                else if (!CppHotReload::IsWorking())
                {
                    CppHotReload::Update();
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                }
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
                    CppHotReload::Init(CppHotReload::ParsingFileProgress, CppHotReload::BeginReload, CppHotReload::EndReload, CppHotReload::FilterFileToBuild, CppHotReload::WillCompileFile, CppHotReload::DidCompileFile, CppHotReload::WillLinkFile, CppHotReload::DidLinkFile, CppHotReload::DidAddType, CppHotReload::DidRemoveType, CppHotReload::DidReloadInstance, CppHotReload::DidReloadVarManager, CppHotReload::DidFatalErrorScript, LogFatal, LogCritical, LogError, LogWarning, LogSystem);

                    if (!CppHotReload::IsInitialized())
                    {
                        LogFatal("Something went wrong initializing C++ HotReload!\n");
                    }
                    else
                    {
                        thCppHotReload = std::thread([&]()
                        {
                            UpdateHotReload();
                        });
                        thCppHotReload.detach();
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
                    didFinish = true;
                    if (thCppHotReload.joinable())
                    {
                        thCppHotReload.join();
                    }
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
            RegisterEntityComponents(entityId);
        }
        void OnEntityDestroyed(const AZ::EntityId& entityId) override
        {
            UnregisterEntityComponents(entityId);
        }
    };
}

// DO NOT MODIFY THIS LINE UNLESS YOU RENAME THE GEM
// The first parameter should be GemName_GemIdLower
// The second should be the fully qualified name of the class above
AZ_DECLARE_MODULE_CLASS(CppHotReload_29f2b034d17d42cca8a3f3cfe05a706f, CppHotReload::CppHotReloadModule)
