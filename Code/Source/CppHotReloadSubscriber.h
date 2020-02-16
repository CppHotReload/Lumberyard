//
//  Copyright (c) since 2014 - 2020 C++ Hot Reload SL. All rights reserved
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
    enum class ComponentType
    {
        EDITOR,
        GENERIC,
        INSTANCE
    };
    struct HotReloadPtr
    {
        AZStd::string guid;
        AZ::Entity* entity;
        AZ::Component* component;
        ComponentType type;
    };
    extern AZStd::vector<HotReloadPtr> hotReloadSubscribers;
    extern AZStd::vector<HotReloadPtr> hotReloadSubscribersToUnsubscribe;

    void SubscribeToHotReload(const HotReloadPtr& hotReloadPtr);
    AZStd::string GetGuidFromSubscriber(AZ::Component* component);
    void UpdateHotReloadSubscriber(const HotReloadPtr& hotReloadPtr);
    void UnsubscribeToHotReload(AZ::Component* component, ComponentType type);

    void* GetGlobalEnvironment();
}
