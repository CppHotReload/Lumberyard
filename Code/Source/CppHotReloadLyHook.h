#include <IGem.h>
#include <AzCore/Component/Component.h>

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
            AZ::ComponentDescriptor* descriptor = GetCurrentDescriptor<$(ModuleName)>();
            descriptor->ReleaseDescriptor();	// Deletes and "un-reflects" the descriptor
            //
            // Prepare to register the new component descriptor following the standards of Ly
            //
            m_descriptors.push_back($(ModuleName)::CreateDescriptor());
        }
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{ };
        }
    };
    AZ_DECLARE_MODULE_CLASS($(ModuleName)_Module_$(Version), CppHotReload::$(ModuleName)_Module_$(Version))
}
