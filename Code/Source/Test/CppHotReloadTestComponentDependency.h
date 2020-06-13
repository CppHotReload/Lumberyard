//
//  Copyright (c) since 2014 - 2020 C++ Hot Reload SL. All rights reserved
//

#pragma once
#include <AzCore/Component/Component.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/EBus/EBus.h>
#include <AzCore/Component/TransformBus.h>
#include <AzCore/Component/TickBus.h>
#include <AzCore/Math/Transform.h>
#include <AzFramework/Entity/GameEntityContextBus.h>

class CppHotReloadTestComponentDependency :       
	public AZ::Component,
	public AZ::TickBus::Handler,
	public AZ::TransformNotificationBus::Handler
{
public:
      AZ_COMPONENT(CppHotReloadTestComponentDependency, "{1F43A13C-AA8B-424F-B80F-6D253BBF4D2A}", AZ::Component);
	  //
	  // Dependency test
	  //
	  static float variableToDepend;

	  void Init() override;
	  void Activate() override;
	  void Deactivate() override;
	  void OnTick(float deltaTime, AZ::ScriptTimePoint time) override;
	  static void Reflect(AZ::ReflectContext* context);

private:
	float m_moveSpeed;
	Vec2 m_movement;
	AZ::Vector3 m_initialPosition;
};
