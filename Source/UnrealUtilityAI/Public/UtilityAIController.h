// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UtilityAIComponent.h"
#include "UtilityAIController.generated.h"

/**
 * 
 */
UCLASS(ComponentWrapperClass)
class UNREALUTILITYAI_API AUtilityAIController : public AAIController
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UUtilityAIComponent* UtilityAIComponent;

	
};
