# Lumberyard
- Prototype plugin for Ly. 
- C++ Hot Reload works on Win64 and macOS. This plugin only supports Win64 and AZ::Component right now.
- **Enter in the discord server if you need help: https://discord.gg/9gsaNnD**

## Before the installation steps
At the moment you'll need comment 1 line in the source code of Ly. There is no other solution as Components has no public method to SetEntity(nullptr). As the new build version restore the base classes, SwapComponents will assert. Basically open Entity.h and search for ::SwapComponents, make it look like this:
```
bool Entity::SwapComponents(Component* componentToRemove, Component* componentToAdd)
{
    AZ_Assert(componentToRemove && componentToAdd, "Invalid component");
    AZ_Assert(CanAddRemoveComponents(), "Can't remove component while the entity is active!");
    AZ_Assert(!componentToRemove->GetEntity() || componentToRemove->GetEntity() == this, "Component doesn't belong to this entity %p [0x%llx] it belongs to %p [0x%llx]", this, m_id, componentToRemove->m_entity, componentToRemove->m_entity->GetId());
    //AZ_Assert(componentToAdd->GetEntity() == nullptr, "Component already belongs to this entity %p [0x%llx]", componentToAdd->m_entity, componentToAdd->m_entity->GetId());

    if (!CanAddRemoveComponents())
```


## Installation
1. Download this repository and place it on the Gems folder
2. Configure your project to include C++ Hot Reload Gem
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
	{ "DevDir"	     , LUMBERYARD_DEV_DIR },
	{ "ThirdPartyDir", LUMBERYARD_THIRDPARTY_DIR },
};
```
3. Follow the pattern adding your libraries and include paths
4. Now navigate to gems_dir/CppHotReload/Code/Source and open IncludeDirectories.txt
5. Add your paths there following the pattern

## More info
To have a bit more information check the documentation of UE4. The version of C++ Hot Reload for Ly is more modern, however all the documentation regarding C++ Hot Reload is valid for Lumberyard.
www.hotreload.tech/UE4
