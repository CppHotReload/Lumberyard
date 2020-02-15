#pragma once
//
// Lumberyard
//
#include <AzCore/Component/Entity.h>
#include <AzCore/Component/EntityBus.h>
#include <AzCore/Component/EntityUtils.h>
#include <AzCore/Component/Component.h>
#include <AzCore/Component/ComponentApplicationBus.h>
//
// C/C++
//
#include <type_traits>

#define CPP_HOT_RELOAD_OVERRIDE_STD_NEW_AND_DELETE 1
#if CPP_HOT_RELOAD_OVERRIDE_STD_NEW_AND_DELETE
namespace CppHotReload
{
	template<typename T>
	static AZ::ComponentDescriptor* CreateDescriptor()                                                                                  
	{                                                                                                                                   
		AZ::ComponentDescriptor* descriptor = nullptr;                                                                              
		AZ::ComponentDescriptorBus::EventResult(descriptor, T::RTTI_Type(), &AZ::ComponentDescriptor::GetDescriptor); 
		if (descriptor)
		{                                                                                                                           
			/* Compare strings first, then pointers.  If we compare pointers first, different strings will give the wrong error message */ 
			if (strcmp(descriptor->GetName(), T::RTTI_TypeName()) != 0)                                               
			{                                                                                                                       
				AZ_Error("Component", false, "Two different components have the same UUID (%s), which is not allowed.\n"            
					"Change the UUID on one of them.\nComponent A: %s\nComponent B: %s",                                            
					T::RTTI_Type().ToString<AZStd::string>().c_str(), descriptor->GetName(), T::RTTI_TypeName());          
				return nullptr;                                                                                                     
			}                                                                                                                       
			else if (descriptor->GetName() != T::RTTI_TypeName())                                                     
			{                                                                                                                       
				AZ_Error("Component", false, "The same component UUID (%s) / name (%s) was registered twice.  This isn't allowed, " 
					"it can cause lifetime management issues / crashes.\nThis situation can happen by declaring a component "  
					"in a header and registering it from two different Gems.\n",                                               
					T::RTTI_Type().ToString<AZStd::string>().c_str(), descriptor->GetName());                         
				return nullptr;                                                                                                     
			}                                                                                                                       
			return descriptor;                                                                                                      
		}                                                                                                                           
		return aznew T::DescriptorType;                                                                                                
	}


	template <typename T>
	static void DefaultNew(T*& NewClass, T*& OldClass, const char* varName, void* userData)
	{
		if (!AZ::Environment::IsReady())
		{
			//
			// Redirect allocators and register our current built class
			//
			InitializeDynamicModule(userData);
			AZ::Module* moduleDataPtr = CreateModuleClass();
			moduleDataPtr->RegisterComponentDescriptors();
		}
		NewClass = aznew T;
	}

	template <typename T>
	static void DefaultDelete(T*& CurrentClass, const char* varName, void* userData)
	{
		
	}
	//
	// In this event the class is created and variables are restored
	//
	template <typename T>
	static void PostHotReload(T*& ptr, const char* varName, void* userData)
	{
	}
}
#endif