# Disclaimer
C++ Hot Reload for Ly 1.24 includes a hack in CppHotReloadLyHook.h to resolve 2 static variables symbols that were introduced in the new version. Additionally in Ly 1.24 the capabilities to set breakpoints after change the code doesn't work. Sorry for this, unless Ly devs get in contact with me and invest time I doubt I'll be able to fix it. Let's be patience and trust in the near future things can change.
Follow the thread here: https://forums.awsgametech.com/t/c-hot-reload-gem-not-working-on-1-24-questions-for-ly-devs/8659

**Please if you are using Ly 1.22 or 1.23 use the proper Git branch**: https://github.com/CppHotReload/Lumberyard/tree/Ly_1.23

# Notes
- Gem tested in **Ly 1.22, Ly 1.23, 1.24** with PCH activated in your project
- If you have a different version, check at the end the seccion: "How to update the Gem"
- C++ Hot Reload works on Win64 and macOS. This plugin only supports Win64 and AZ::Component right now
- **Enter in the discord server if you need help: https://discord.gg/9gsaNnD**
- License: for testing, evaluation or non-commercial purpose projects
  - For more details check COMMUNITY_LICENSE.txt file.
- To obtain a commercial license: contact *at* hotreload.tech

## Installation
1. Download this repository and place the folder CppHotReload on the Gems folder
2. Execute the project configurator and activate the Gem C++ Hot Reload

### Test
The Gem include a Test folder with 1 component as a sample of how to include other folders outside of your default game project
1. Create an empty scene and an entity with a child entity. To the child add a mesh component and select a Sphere or other visible mesh
3. Add the component `C++ Hot Reload - Test`, click the button Add Component and searching for "Test"
4. Press play and see how it moves.
5. Navigate to gems_dir/CppHotReload/Code/Source/Test/CppHotReloadTestComponent.cpp
6. Around the line 52 you'll see
```
//
// C++ Hot Reload
// 
#if CPP_HOT_RELOAD_DEPENDENCY_TEST
	m_movement.x = CppHotReloadTestComponentDependency::variableToDepend;
#else
	m_movement.x = -150.0f; 
#endif
```
7. Press play in the Editor, or in Edit mode (both works) and change the value to `m_movement.x = 100;`
You'll see that the ball is moving to the other direction!

### How to configure extra libraries
1. Navigate to gems_dir/CppHotReload/Code/Source and open CppHotReloadInit.cpp and search for `lyLibPaths`
2. You'll see that is configured with the basic libraries
3. Follow the pattern adding your libraries and library paths

### How to configure extra include paths
1. Navigate to gems_dir/CppHotReload/Code/Source and open CppHotReloadInit.cpp and search for `EnvironmentVariables`
2. You'll see that is configured with the basic paths
```
Configuration::EnvironmentVariable EnvironmentVariables[] =
{
    { "DevDir"       , LUMBERYARD_DEV_DIR },
    { "ThirdPartyDir", LUMBERYARD_THIRDPARTY_DIR },
    { "BinTemp"      , LUMBERYARD_BIN_TEMP_DIR },
};
```
3. Follow the pattern adding your libraries and include paths
4. Now navigate to gems_dir/CppHotReload/Code/Source and open IncludeDirectories.txt
5. Add your paths there following the pattern

## More info
To have a bit more information check the documentation of UE4. The version of C++ Hot Reload for Ly is more modern, however all the documentation regarding C++ Hot Reload is valid for Lumberyard.
www.hotreload.tech/docs/ue4

## Youtube playlist about the integration
https://www.youtube.com/playlist?list=PLHW7E3f3ce1kiKQwqlRx804_JEH-vOpbg

# Good practices to reduce hot reload time
How to easily cut by half the reloading time?
First off, my machine: Corei7-4790 CPU @ 3.60GHz, 3601 Mhz, 4 Core(s), 8 Logical Processor(s) - and the project in a SSD.
A very common machine, however C++ Hot Reload will depend on your build config, reloading the default written component `WayPointManagerComponent` takes around **8-10 seconds**. 

The current situation on the include files in `WayPointManagerComponent.h` file are:
```
#include <AzCore/Component/Component.h>
#include <AzCore/Component/Entity.h>
#include <AzCore/Math/Vector3.h>
#include <AzCore/Component/TransformBus.h>
```
And the cpp file includes are:
```
#include "StdAfx.h"
#include "WayPointManagerComponent.h"

#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
```

If you move to the PCH file, StdAfx.h, the following includes files as follows
```
... 

#include <AzCore/Component/Component.h>
#include <AzCore/Component/Entity.h>
#include <AzCore/Math/Vector3.h>
#include <AzCore/Component/TransformBus.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include "Game/WayPointManagerBus.h"
```
`WayPointManagerComponent.h` as follows (yes, empty)
```
#pragma once
```
`WayPointManagerComponent.cpp` as follows
```
#include "StdAfx.h"
#include "WayPointManagerComponent.h"
```

C++ Hot Reload will have the following statistics:
- **Reload time:  5.33316 seconds**
- Setup time:   0.0139533 seconds
- Compile time: 3.79052 seconds
- Link time:    1.52869 seconds

**So a big cut in reload time**, this can be even more improved creating a shared pch file for the engine and other strategies until 1-2 seconds of reload time. But that requires an additional effort from Ly developers to provide those types build configurations. 

My personal recommendation is that you make your components normally, keep up-to-date your PCH file and your components includes cleaned without much cross-include files.

# You are interested in know about these options!
```
{ Configuration::Prameter::SEARCH_DEPENDENCIES_ON_DEMAND, "NO" },		// if you not always use EBuses
{ Configuration::Prameter::SEARCH_DEPENDENCIES_ON_DEMAND_FAST, "YES" },		// dependencies are found if are .h or .cpp only
{ Configuration::Prameter::SEARCH_DEPENDENCIES_ON_DEMAND_WHEN_V_0, "NO" },	// search always dependencies
```
Lumberyard is designed to use **Ebuses and avoid dependencies between components based on include files**. Just in case you have them for legacy reasons, C++ Hot Reload can search for you those dependencies and dependats and build them. That have a cost, and you'll need to "pay" that cost at least the first time you reload a file to have all symbols. 

If that's your case activate: SEARCH_DEPENDENCIES_ON_DEMAND = "YES", my recommendation: use EBuses :)

# How to update the Gem
Not every build step is automatic in the Gem, sadly. **But easy to fix**.
Navigate to gems_dir/CppHotReload/Code/Source and open CppHotReloadInit.cpp and search for `3rd party`, Ly developers probably bump the versions of these libraries with every new release. Update the paths accordingly.
```
//
// 3rd party
//
additionalLibPaths += "/LIBPATH:\"" + GetThirdPartyDir() + "/Lua/5.1.1.9-az/build/win_x64/vc140/release\" ";
additionalLibPaths += "/LIBPATH:\"" + GetThirdPartyDir() + "/zstd/1.35-pkg.1/build/win_x64/vc140/release\" ";
additionalLibPaths += "/LIBPATH:\"" + GetThirdPartyDir() + "/lz4/r128-pkg.3/build/win_x64/vc140/release\" ";
additionalLibPaths += "/LIBPATH:\"" + GetThirdPartyDir() + "/zlib/1.2.8-pkg.3/build/win_x64/vc140/release\" ";
```

# Honourable Mention
@FluffyQuaternion for the early stage testing and his ... "special" project configuration :) 
