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