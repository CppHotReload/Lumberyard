#pragma once
//
// Ly
//
#include <AzCore/Component/Component.h>
#include <AzCore/Component/TickBus.h>
//
// Gem
//
#include <CppHotReload/CppHotReloadBus.h>

namespace CppHotReload
{
    class CppHotReloadSystemComponent
        : public AZ::Component
        , protected CppHotReloadRequestBus::Handler
        , protected AZ::SystemTickBus::Handler
    {
    public:
        AZ_COMPONENT(CppHotReloadSystemComponent, "{80DAEBD8-8554-497C-A7A7-9103D419465E}");

        static void Reflect(AZ::ReflectContext* context);

        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        void OnSystemTick() override;
        
    protected:
        void Init() override;
        void Activate() override;
        void Deactivate() override;

    };
}
