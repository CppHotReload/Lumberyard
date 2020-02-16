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
}
