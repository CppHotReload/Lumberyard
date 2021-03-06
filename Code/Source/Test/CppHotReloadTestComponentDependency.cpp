//
//  Copyright (c) since 2014 - 2020 C++ Hot Reload SL. All rights reserved
//

#pragma once
#include "StdAfx.h"
#include "CppHotReloadTestComponentDependency.h"

float CppHotReloadTestComponentDependency::variableToDepend = -150.f; 

void CppHotReloadTestComponentDependency::Init()                 
{          
	m_moveSpeed = 0.1f;
	m_movement = ZERO;      
}     
  
void CppHotReloadTestComponentDependency::Activate()              
{  
	AZ::TickBus::Handler::BusConnect();   
	AZ::TransformNotificationBus::Handler::BusConnect(GetEntityId()); 


	AZ::Transform localTransform = AZ::Transform::Identity();
	EBUS_EVENT_ID_RESULT(localTransform, GetEntityId(), AZ::TransformBus, GetLocalTM);

	m_initialPosition = localTransform.GetPosition();     
} 
 
void CppHotReloadTestComponentDependency::Deactivate()  
{ 
	AZ::TickBus::Handler::BusDisconnect(); 
	AZ::TransformNotificationBus::Handler::BusDisconnect();
} 
   
void CppHotReloadTestComponentDependency::OnTick(float deltaTime, AZ::ScriptTimePoint time) 
{
	AZ::Transform localTransform = AZ::Transform::Identity();
	EBUS_EVENT_ID_RESULT(localTransform, GetEntityId(), AZ::TransformBus, GetLocalTM);

	const AZ::Vector3 currPosition = localTransform.GetPosition();
	//
	// Let's loop the transformation
	//
	if (currPosition.GetLength() >= 10.0f)
	{
		localTransform.SetPosition(m_initialPosition);
		EBUS_EVENT_ID(GetEntityId(), AZ::TransformBus, SetLocalTM, localTransform);  
	}
	else 
	{
		//
		// C++ Hot Reload
		// 
		m_movement.x = -100.0f; 

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

void CppHotReloadTestComponentDependency::Reflect(AZ::ReflectContext* context)
{
	AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(context);
	if (serializeContext) 
	{
		serializeContext->Class<CppHotReloadTestComponentDependency, AZ::Component>()
			->Version(5)
			->Field("Movement Speed", &CppHotReloadTestComponentDependency::m_moveSpeed)
			;

		AZ::EditContext* editContext = serializeContext->GetEditContext();
		if (editContext)
		{
			editContext->Class<CppHotReloadTestComponentDependency>("C++ Hot Reload - Test Dependency", "The World's Most Clever Component Dependency")
				->ClassElement(AZ::Edit::ClassElements::EditorData, "")
				->Attribute("Category", "Test")
				->Attribute("AutoExpand", true)
				->Attribute("AppearsInAddComponentMenu", AZ_CRC("Game", 0x232b318c))
				->DataElement(1, &CppHotReloadTestComponentDependency::m_moveSpeed, "Move Speed", "Speed at which the camera moves")
				->Attribute("Min", 0.0f)
				->Attribute("Max", 10.0f)
				->Attribute("ChangeNotify", AZ_CRC("RefreshValues", 0x28e720d4))
				;
		}
	}
}
