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

	UUtilityAIAction();

	virtual UWorld* GetWorld() const override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Spawn"))
	void ReceiveSpawn(AAIController* Controller);

	virtual void Spawn(AAIController* Controller);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Tick"))
	void ReceiveTick(float DeltaTime, AAIController* Controller, APawn* Pawn);

	virtual void Tick(float DeltaTime, AAIController* Controller, APawn* Pawn);

	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Can Run"))
	bool ReceiveCanRun(AAIController* Controller, APawn* Pawn) const;

	bool ReceiveCanRun_Implementation(AAIController* Controller, APawn* Pawn) const { return true; }

	virtual bool CanRun(AAIController* Controller, APawn* Pawn) const;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Score"))
	float ReceiveScore(AAIController* Controller, APawn* Pawn);

	virtual float Score(AAIController* Controller, APawn* Pawn);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Enter"))
	void ReceiveEnter(AAIController* Controller, APawn* Pawn);

	virtual void Enter(AAIController* Controller, APawn* Pawn);

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="Exit"))
	void ReceiveExit(AAIController* Controller, APawn* Pawn);

	virtual void Exit(AAIController* Controller, APawn* Pawn);

	UPROPERTY(BlueprintReadOnly)
	float LastScore;

	UPROPERTY(BlueprintReadOnly)
	bool LastCanRun;

	UFUNCTION(BlueprintCallable)
	void Kill();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsMarkedForDeath();

	UFUNCTION(BlueprintCallable)
	void Resurrect();

private:
	bool bMarkedForDeath;
	
};
