// Copyright 2018 Roberto De Ioris

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/Engine/Blueprint.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "UtilityAIAction.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class UNREALUTILITYAI_API UUtilityAIAction : public UObject
{
	GENERATED_BODY()

public:

	virtual UWorld* GetWorld() const override;

	UFUNCTION(BlueprintImplementableEvent)
	void Tick(float DeltaTime, AAIController* Controller, APawn* Pawn);

	UFUNCTION(BlueprintNativeEvent)
	bool CanRun(AAIController* Controller, APawn* Pawn) const;

	bool CanRun_Implementation(AAIController* Controller, APawn* Pawn) const { return true; }

	UFUNCTION(BlueprintImplementableEvent)
	float Score(AAIController* Controller, APawn* Pawn);

	UFUNCTION(BlueprintImplementableEvent)
	void Enter(AAIController* Controller, APawn* Pawn);

	UFUNCTION(BlueprintImplementableEvent)
	void Exit(AAIController* Controller, APawn* Pawn);

	UPROPERTY(BlueprintReadOnly)
	float LastScore;
	
};
