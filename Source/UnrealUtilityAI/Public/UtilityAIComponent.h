// Copyright 2018 Roberto De Ioris

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UtilityAIAction.h"
#include "UtilityAIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUtilityAIActionSpawned, UUtilityAIAction*, Action);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUtilityAIActionChanged, UUtilityAIAction*, NewAction, UUtilityAIAction*, OldAction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUtilityAIActionNotAvailable);


UCLASS(BlueprintType, Blueprintable, ClassGroup = (AI), meta = (BlueprintSpawnableComponent))
class UNREALUTILITYAI_API UUtilityAIComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UUtilityAIComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility AI")
	TSet<TSubclassOf<UUtilityAIAction>> Actions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility AI")
	bool bIgnoreZeroScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility AI")
	bool bUseLowestScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Utility AI")
	bool bInvertPriority;

	UPROPERTY(BlueprintAssignable, Category = "Utility AI", meta = (DisplayName = "On UtilityAI Action Spawned"))
	FUtilityAIActionSpawned OnUtilityAIActionSpawned;

	UPROPERTY(BlueprintAssignable, Category = "Utility AI", meta = (DisplayName = "On UtilityAI Action Changed"))
	FUtilityAIActionChanged OnUtilityAIActionChanged;

	UPROPERTY(BlueprintAssignable, Category = "Utility AI", meta = (DisplayName = "On UtilityAI Action Not Available"))
	FUtilityAIActionNotAvailable OnUtilityAIActionNotAvailable;

	UPROPERTY()
	TSet<UUtilityAIAction*> InstancedActions;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Utility AI")
	TArray<UUtilityAIAction*> GetActionInstances() const;

protected:

	UUtilityAIAction * LastAction;
	APawn* LastPawn;

	bool CheckLowestScore(UUtilityAIAction* Current, UUtilityAIAction* Best) const;
	bool CheckHighestScore(UUtilityAIAction* Current, UUtilityAIAction* Best) const;
};
