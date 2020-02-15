#include "StdAfx.h"
#include <CppHotReloadSystemComponent.h>
#include <ISystem.h>
#include <CryLibrary.h>
//
// AzCore
//
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/EditContextConstants.inl>
//
// C++ Hot Reload
//
#include "CppHotReload/HotReloadAPI.h"

namespace CppHotReload
{
    void CppHotReloadSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (AZ::SerializeContext* serialize = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serialize->Class<CppHotReloadSystemComponent, AZ::Component>()
                ->Version(0);

            if (AZ::EditContext* ec = serialize->GetEditContext())
            {
                ec->Class<CppHotReloadSystemComponent>("CppHotReload", "[Description of functionality provided by this System Component]")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                        ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("System", 0xc94d118b))
                        ->Attribute(AZ::Edit::Attributes::AutoExpand, true)
                    ;
            }
        }
    }

    void CppHotReloadSystemComponent::OnSystemTick()
    {
        if (CppHotReload::IsInitialized())
        {
            if (!CppHotReload::IsWorking())
            {
                //CppHotReload::HotReload();
                CppHotReload::Update();
                CppHotReload::Purge();
            }
        }
    }

    void CppHotReloadSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC("CppHotReloadService", 0x631ad0f6));
    }

    void CppHotReloadSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC("CppHotReloadService", 0x631ad0f6));
    }

    void CppHotReloadSystemComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        AZ_UNUSED(required);
    }

    void CppHotReloadSystemComponent::GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        AZ_UNUSED(dependent);
    }

    void CppHotReloadSystemComponent::Init()
    {

    }

    void CppHotReloadSystemComponent::Activate()
    {
        CppHotReloadRequestBus::Handler::BusConnect();
    }

    void CppHotReloadSystemComponent::Deactivate()
    {
        CppHotReloadRequestBus::Handler::BusDisconnect();
    }
}
