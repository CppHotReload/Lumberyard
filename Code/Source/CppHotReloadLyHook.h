//
//  Copyright (c) since 2014 - 2020 C++ Hot Reload SL. All rights reserved
//

//
// Environment Variables available:
// FileName -> $(FileName)
// Name -> $(Name)
// ModuleName -> $(ModuleName)
// TypeName -> $(TypeName)
// KeyName -> $(KeyName)
// Uuid -> $(Uuid)
// Version -> $(Version)
//
#include "StdAfx.h"
#include "$(FileName)"
#include <platform_impl.h>
#include <IGem.h>
#include <AzCore/Component/Component.h>
//
// Ly 1.24 HACK! - BEGIN
// More info: https://forums.awsgametech.com/t/c-hot-reload-gem-not-working-on-1-24-questions-for-ly-devs/8659
//
#include <AzCore/Console/ConsoleFunctor.h>

namespace AZ
{
    ConsoleFunctorBase* ConsoleFunctorBase::s_deferredHead = nullptr;
    bool ConsoleFunctorBase::s_deferredHeadInvoked = false;
}
//
// Ly 1.24 HACK! - BEGIN
//

namespace CppHotReload
{
    template<typename T>
    static AZ::ComponentDescriptor* GetCurrentDescriptor()                                                                                  
    {                                                                                                                                   
        AZ::ComponentDescriptor* descriptor = nullptr;                                                                              
        AZ::ComponentDescriptorBus::EventResult(descriptor, T::RTTI_Type(), &AZ::ComponentDescriptor::GetDescriptor); 
        return descriptor;                                                                                                      
    }

    class $(ModuleName)_Module_$(Version)
    : public CryHooksModule
    {
    public:
        AZ_RTTI($(ModuleName)_Module_$(Version), "{00000000-0000-0000-0000-000000000001}", CryHooksModule);

        $(ModuleName)_Module_$(Version)()
        : CryHooksModule()
        {
            //
            // Say goodbye to the previous component
            //
            AZ::ComponentDescriptor* descriptor = GetCurrentDescriptor<$(KeyName)>();
            if (descriptor)
                descriptor->ReleaseDescriptor();	// Deletes and "un-reflects" the descriptor
            //
            // Prepare to register the new component descriptor following the standards of Ly
            //
            m_descriptors.push_back($(KeyName)::CreateDescriptor());
        }
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{ };
        }
    };
    AZ_DECLARE_MODULE_CLASS($(ModuleName)_Module_$(Version), CppHotReload::$(ModuleName)_Module_$(Version))
}
