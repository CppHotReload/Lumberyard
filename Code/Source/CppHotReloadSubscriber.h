//
//  Copyright (c) 2014 - 2019 C++ Hot Reload SL. All rights reserved
//
//  This file is subject to the terms and conditions defined in
//  file 'LICENSE.txt', which is part of this source code package.
//

#pragma once
//
// Ly
//
#include <AzCore/std/string/string.h>
#include <AzCore/std/containers/vector.h>

namespace AZ
{
    class Component;
    class Entity;
}
namespace CppHotReload
{
    //
    // TODO: only for the prototype, it's used in Events
    //
    struct HotReloadPtr
    {
        AZStd::string guid;
        AZ::Entity* entity;
        AZ::Component* component;
    };
    extern AZStd::vector<HotReloadPtr> hotReloadSubscribers;

    void SubscribeToHotReload(const HotReloadPtr& hotReloadPtr);
    void UpdateHotReloadSubscriber(const HotReloadPtr& hotReloadPtr);
    void UnsubscribeToHotReload(AZ::Component*);

    void* GetGlobalEnvironment();
}
