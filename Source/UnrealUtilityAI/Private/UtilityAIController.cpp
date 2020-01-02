// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityAIController.h"

AUtilityAIController::AUtilityAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

	UtilityAIComponent = CreateDefaultSubobject<UUtilityAIComponent>("UtilityAIComponent");
}
