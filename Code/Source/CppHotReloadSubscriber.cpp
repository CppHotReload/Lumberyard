//
//  Copyright (c) since 2014 - 2020 C++ Hot Reload SL. All rights reserved
//

#include "StdAfx.h"
#include "CppHotReloadSubscriber.h"
//
// Ly
//
#include <AzCore/Component/Entity.h>
#include <AzCore/Component/Component.h>

AZStd::vector<CppHotReload::HotReloadPtr> CppHotReload::hotReloadSubscribers;

namespace CppHotReload
{
    void* GetGlobalEnvironment()
    {
        return AZ::Environment::GetInstance();
    }

    void SubscribeToHotReload(const HotReloadPtr& hotReloadPtr)
    {
        hotReloadSubscribers.emplace_back(hotReloadPtr);
    }

    AZStd::string GetGuidFromSubscriber(AZ::Component* component)
    {
        for (AZStd::vector<HotReloadPtr>::iterator it = hotReloadSubscribers.begin(); it != hotReloadSubscribers.end(); ++it)
        {
            if (it->component == component)
            {
                return it->guid;
            }
        }
        return "";
    }

    void UpdateHotReloadSubscriber(const HotReloadPtr& hotReloadPtr)
    {
        for (AZStd::vector<HotReloadPtr>::iterator it = hotReloadSubscribers.begin(); it != hotReloadSubscribers.end(); ++it)
        {
            if (it->guid == hotReloadPtr.guid)
            {
                it->component = hotReloadPtr.component;
                return;
            }
        }
    }

    void UnsubscribeToHotReload(AZ::Component* component, ComponentType type)
    {
        for (AZStd::vector<HotReloadPtr>::iterator it = hotReloadSubscribers.begin(); it != hotReloadSubscribers.end(); ++it)
        {
            if (it->component == component)
            {
                hotReloadSubscribers.erase(it);
                return;
            }
        }
    }

    AZ::Entity* GetEntityFromSubscribers(const AZ::EntityId& entityId)
    {
        for (AZStd::vector<HotReloadPtr>::iterator it = hotReloadSubscribers.begin(); it != hotReloadSubscribers.end(); ++it)
        {
            AZ::Entity* entity = it->entity;
            if (entity->GetId() == entityId)
            {
                return entity;
            }
        }
        return nullptr;
    }

    void UnregisterComponentsByEntityId(const AZ::EntityId& entityId)
    {
        for (AZStd::vector<HotReloadPtr>::iterator it = hotReloadSubscribers.begin(); it != hotReloadSubscribers.end(); )
        {
            if (it->entityId == entityId)
            {
                const AZStd::string& guid = CppHotReload::GetGuidFromSubscriber(it->component);
                if (!guid.empty())
                {
                    AZ_Printf("C++ Hot Reload", "UnregisterComponent [%s]::[%s] - id: %s\n", it->entityName.c_str(), it->typeName.c_str(), it->guid.c_str());

                    CppHotReload::UnregisterPtr(it->typeName.c_str(), reinterpret_cast<void*>(it->component), guid.c_str());
                    it = hotReloadSubscribers.erase(it);
                    continue;
                }
            }
            ++it;
        }
    }
}
