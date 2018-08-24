// Copyright 2018 Roberto De Ioris

#include "UtilityAIAction.h"

UWorld* UUtilityAIAction::GetWorld() const
{
	UObject* Outer = GetOuter();
	if (!Outer)
		return nullptr;
	AAIController* Controller = Cast<AAIController>(GetOuter());
	if (!Controller)
		return nullptr;

	return Controller->GetWorld();
}


