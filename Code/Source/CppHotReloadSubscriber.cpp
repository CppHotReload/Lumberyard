//
//  Copyright (c) 2014 - 2019 C++ Hot Reload SL. All rights reserved
//
//  This file is subject to the terms and conditions defined in
//  file 'LICENSE.txt', which is part of this source code package.
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

    void UnsubscribeToHotReload(AZ::Component* component)
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
