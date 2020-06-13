//
//  Copyright (c) since 2014 - 2020 C++ Hot Reload SL. All rights reserved
//

#pragma once

namespace AZ
{
    class Component;
    class Entity;
}
namespace AzToolsFramework
{
    namespace Components
    {
        class GenericComponentWrapper;
    }
}

namespace CppHotReload
{
    enum class ComponentType
    {
        UNKNOWN,
        EDITOR,
        GENERIC,
        INSTANCE
    };
    struct HotReloadPtr
    {
        AZStd::string guid;
        AZStd::string typeName;
        AZStd::string entityName;
        AZ::EntityId entityId;
        AZ::Entity* entity;
        AZ::Component* component;
        AzToolsFramework::Components::GenericComponentWrapper* genericComponentWrapper;
        ComponentType type;
    };
    extern AZStd::vector<HotReloadPtr> hotReloadSubscribers;
    extern AZStd::vector<HotReloadPtr> hotReloadSubscribersToUnsubscribe;

    void SubscribeToHotReload(const HotReloadPtr& hotReloadPtr);
    AZStd::string GetGuidFromSubscriber(AZ::Component* component);
    void UpdateHotReloadSubscriber(const HotReloadPtr& hotReloadPtr);
    void UnsubscribeToHotReload(AZ::Component* component, ComponentType type);
    AZ::Entity* GetEntityFromSubscribers(const AZ::EntityId& entityId);
    void UnregisterComponentsByEntityId(const AZ::EntityId& entityId);
    void* GetGlobalEnvironment();
}
