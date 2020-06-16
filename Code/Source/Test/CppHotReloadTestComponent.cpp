//
//  Copyright (c) since 2014 - 2020 C++ Hot Reload SL. All rights reserved
//

#pragma once
#include "StdAfx.h"
#include "CppHotReloadTestComponent.h"

CppHotReloadTestComponent::CppHotReloadTestComponent()
{
	AZ_Printf("C++ Hot Reload", "CppHotReloadTestComponent::ctor: %p", this);
}

CppHotReloadTestComponent::~CppHotReloadTestComponent()
{
	AZ_Printf("C++ Hot Reload", "CppHotReloadTestComponent::dtor: %p", this);
}

void CppHotReloadTestComponent::Init()                 
{           
	m_moveSpeed = 0.1f;
	m_movement = ZERO;       
}     
  
void CppHotReloadTestComponent::Activate()              
{  
	AZ_Printf("C++ Hot Reload", "CppHotReloadTestComponent::Activate: %p", this);
	AZ::TickBus::Handler::BusConnect();   
	AZ::TransformNotificationBus::Handler::BusConnect(GetEntityId()); 


	AZ::Transform localTransform = AZ::Transform::Identity();
	EBUS_EVENT_ID_RESULT(localTransform, GetEntityId(), AZ::TransformBus, GetLocalTM);

	m_initialPosition = localTransform.GetPosition();     
} 
 
void CppHotReloadTestComponent::Deactivate()  
{ 
	AZ_Printf("C++ Hot Reload", "CppHotReloadTestComponent::Deactivate: %p", this);
	AZ::TickBus::Handler::BusDisconnect(); 
	AZ::TransformNotificationBus::Handler::BusDisconnect();
} 
   
void CppHotReloadTestComponent::OnTick(float deltaTime, AZ::ScriptTimePoint time) 
{
	AZ::Transform localTransform = AZ::Transform::Identity();
	EBUS_EVENT_ID_RESULT(localTransform, GetEntityId(), AZ::TransformBus, GetLocalTM);

	const AZ::Vector3 currPosition = localTransform.GetPosition();
	//
	// Let's loop the transformation
	//
	const float currLength = currPosition.GetLength();
	const float initialLength = m_initialPosition.GetLength();

	const float maxDistance = 10.0f;   

	if (currLength >= initialLength + maxDistance || currLength <= initialLength - maxDistance)   
	{ 
		localTransform.SetPosition(m_initialPosition);
		EBUS_EVENT_ID(GetEntityId(), AZ::TransformBus, SetLocalTM, localTransform);  
	}
	else 
	{
		//
		// C++ Hot Reload
		//      
#if CPP_HOT_RELOAD_DEPENDENCY_TEST
		m_movement.x = CppHotReloadTestComponentDependency::variableToDepend; 
#else
		m_movement.x = -150.0f;
#endif
		// Update movement
		const float moveSpeed         = m_moveSpeed * deltaTime;
		const AZ::Vector3 right		  = localTransform.GetColumn(0);
		const AZ::Vector3 forward	  = localTransform.GetColumn(1);
		const AZ::Vector3 movement	  = (forward * m_movement.y) + (right * m_movement.x);
		const AZ::Vector3 newPosition = currPosition + (movement * moveSpeed);
		localTransform.SetPosition(newPosition);

		EBUS_EVENT_ID(GetEntityId(), AZ::TransformBus, SetLocalTM, localTransform);
	}
}   

void CppHotReloadTestComponent::Reflect(AZ::ReflectContext* context)
{
	AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(context);
	if (serializeContext) 
	{
		serializeContext->Class<CppHotReloadTestComponent, AZ::Component>()
			->Version(5)
			->Field("Movement Speed", &CppHotReloadTestComponent::m_moveSpeed)
			;

		AZ::EditContext* editContext = serializeContext->GetEditContext();
		if (editContext)
		{
			editContext->Class<CppHotReloadTestComponent>("C++ Hot Reload - Test", "The World's Most Clever Component")
				->ClassElement(AZ::Edit::ClassElements::EditorData, "")
				->Attribute("Category", "Test")
				->Attribute("AutoExpand", true)
				->Attribute("AppearsInAddComponentMenu", AZ_CRC("Game", 0x232b318c))
				->DataElement(1, &CppHotReloadTestComponent::m_moveSpeed, "Move Speed", "Speed at which the camera moves")
				->Attribute("Min", 0.0f)
				->Attribute("Max", 10.0f)
				->Attribute("ChangeNotify", AZ_CRC("RefreshValues", 0x28e720d4))
				;
		}
	}
}
