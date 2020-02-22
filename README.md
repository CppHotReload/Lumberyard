# Lumberyard
- Prototype plugin for Ly. 
- C++ Hot Reload works on Win64 and macOS. This plugin only supports Win64 and AZ::Component right now.
- **Enter in the discord server if you need help: https://discord.gg/9gsaNnD**

## Installation
1. Download this repository and place the folder CppHotReload on the Gems folder
2. Execute the project configurator and activate the Gem C++ Hot Reload

## How to add libraries
1. Navigate to gems_dir/CppHotReload/Code/Source and open CppHotReloadInit.cpp and search for `lyLibPaths`
2. You'll see that is configured with the basic libraries
3. Follow the pattern adding your libraries and library paths

## How to add include paths
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
www.hotreload.tech/UE4

## Youtube playlist for the prototype integration
https://www.youtube.com/playlist?list=PLHW7E3f3ce1kiKQwqlRx804_JEH-vOpbg

# Good practices
How to easily cut by half the reloading time?
First off, my machine: Corei7-4790 CPU @ 3.60GHz, 3601 Mhz, 4 Core(s), 8 Logical Processor(s) - and the project in a SSD.
A very common machine, however C++ Hot Reload will depend on your include configuration, reloading the default written component `WayPointManagerComponent` takes around **8-10 seconds**. 

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

If you move to the PCH file, StdAfx.h the following includes files as follows
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

**So a big cut in reload time**, this can be even more improved creating a shared pch file for the engine and other strategies until 1-2 seconds of reload time. But that requires an additional effort from Ly developers to provide those types build configurations. My personal recommendation is that you make your components normally and keep up-to-date your PCH file and your components includes cleaned.
