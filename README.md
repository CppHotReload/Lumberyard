# Lumberyard
- Prototype plugin for Ly. 
- C++ Hot Reload works on Win64 and macOS. This plugin only supports Win64 and AZ::Component right now.
- **Enter in the discord server if you need help: https://discord.gg/9gsaNnD**

## Installation
1. Download this repository and place it on the Gems folder
2. Configure your project to include C++ Hot Reload Gem
   - **You can see the final configuration file for SamplesProject at gems_dir/CppHotReload/Configs/Config_For_SamplesProject.h**
3. Navigate to gems_dir/CppHotReload/Code/Source and open CppHotReloadConfig.h
4. Configure the paths to your project directory
   - C++ Hot Reload gem, comes by default pointing to its Test folder, a Component you can add first to check everything it's ok
   - Modify `LUMBERYARD_ROOT` with your Ly directory
   - After test C++ Hot Reload adding the CppHotReloadTestComponent, let's configure now as an example SamplesProject.
   - Change now `LUMBERYARD_GAME_NAM`E to, for instance: SamplesProject
     * `#define LUMBERYARD_GAME_NAME "SamplesProject"`
   - You'll see a line like follows:
     * `#define LUMBERYARD_GAME_PATH LUMBERYARD_CPP_HOT_RELOAD_DIR "/Code/Source/Test/"`
   - That's the folder that C++ Hot Reload will watch for changes
   - Let's change it for:
     * `#define LUMBERYARD_GAME_PATH LUMBERYARD_DEV_DIR LUMBERYARD_GAME_NAME "/Gem/Code/Source/Game/"`
   - Time to locate the PCH, search for the macro `LUMBERYARD_GAME_PCH_PATH` and change it as follows
     * `#define LUMBERYARD_GAME_PCH_PATH LUMBERYARD_BIN_TEMP_DIR LUMBERYARD_GAME_NAME "/Gem/Code/"`
   - Lumberyard build system package id, or how I call it now: `CPP_HOT_RELOAD_PCH_UNKNOWN_NUMBER` an id I don't know yet how to take it automatically. So please search for that number id when you build your project. In the same PCH folder you'll see a filename like this: `StdAfx.2581533.pch`, copy that number in between `StdAfx.__NUMBER__.pch` and assign that value:
     * `#define CPP_HOT_RELOAD_PCH_UNKNOWN_NUMBER "2581533"`
  
This should be all for SamplesProject. However you project can be more complex than that have other dependencies, 3rdParty libs, etc. Try to follow the UE4 documentation if you have doubts in the link below

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
    { "DevDir"		, LUMBERYARD_DEV_DIR },
    { "ThirdPartyDir"	, LUMBERYARD_THIRDPARTY_DIR },
    { "BinTemp"		, LUMBERYARD_BIN_TEMP_DIR },
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
**Reload time:  5.33316 seconds**
Setup time:   0.0139533 seconds
Compile time: 3.79052 seconds
Link time:    1.52869 seconds

**So kind of the half in reload time**, this can be even more improved creating a shared pch file for the engine and other strategies until 1-2 seconds of reload time. But that requires an additional effort from Ly developers to provide those types build configurations.
