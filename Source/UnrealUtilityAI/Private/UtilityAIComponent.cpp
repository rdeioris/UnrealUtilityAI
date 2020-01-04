// Copyright 2018 Roberto De Ioris

#include "UtilityAIComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"


// Sets default values for this component's properties
UUtilityAIComponent::UUtilityAIComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bCanRunWithoutPawn = true;

	// ...
}


// Called when the game starts
void UUtilityAIComponent::BeginPlay()
{
	Super::BeginPlay();

	LastAction = nullptr;
	LastPawn = nullptr;

	LastSwitchTime = 0;

	// instantiate actions
	for (TSubclassOf<UUtilityAIAction> ActionClass : Actions)
	{
		SpawnActionInstance(ActionClass);
	}

	OnUtilityAIInitialized.Broadcast();
}

bool UUtilityAIComponent::CanSpawnActionInstance(TSubclassOf<UUtilityAIAction> ActionClass) const
{
	for (UUtilityAIAction* Action : InstancedActions)
	{
		if (Action->GetClass() == ActionClass)
			return false;
	}
	return true;
}

UUtilityAIAction* UUtilityAIComponent::SpawnActionInstance(TSubclassOf<UUtilityAIAction> ActionClass)
{
	// skip null
	if (!ActionClass)
		return nullptr;

	// avoid duplicates
	if (!CanSpawnActionInstance(ActionClass))
		return nullptr;

	AAIController* Controller = Cast<AAIController>(GetOwner());
	if (!Controller)
		return nullptr;

	UUtilityAIAction* Action = NewObject<UUtilityAIAction>(Controller, ActionClass);
	InstancedActions.Add(Action);
	Action->Spawn(Controller);
	OnUtilityAIActionSpawned.Broadcast(Action);

	return Action;
}

bool UUtilityAIComponent::InternalRandBool() const
{
	float r = 0;
	if (bUseRandomStream)
		r = RandomStream.FRandRange(0.0f, 1.0f);
	else
		r = FMath::RandRange(0.0f, 1.0f);
	return 0.5f > r;
}

bool UUtilityAIComponent::CheckLowestScore(UUtilityAIAction* Current, UUtilityAIAction* Best) const
{
	// fast case
	if (!Best)
		return true;

	if (FMath::Abs(Best->LastScore - Current->LastScore) <= EqualityTolerance)
	{
		if (bInvertPriority)
			return true;
		if (bRandomizeOnEquality)
		{
			return InternalRandBool();
		}
		return false;
	}

	if (Best->LastScore > Current->LastScore)
	{
		return true;
	}


	return false;
}

bool UUtilityAIComponent::CheckHighestScore(UUtilityAIAction* Current, UUtilityAIAction* Best) const
{
	// fast case
	if (!Best)
		return true;

	if (FMath::Abs(Best->LastScore - Current->LastScore) <= EqualityTolerance)
	{
		if (bInvertPriority)
			return true;
		if (bRandomizeOnEquality)
		{
			return InternalRandBool();
		}
		return false;
	}

	if (Best->LastScore < Current->LastScore)
	{
		return true;
	}


	return false;
}

UUtilityAIAction* UUtilityAIComponent::ReceiveComputeBestAction_Implementation(AAIController* Controller, APawn* Pawn)
{
	UUtilityAIAction* BestAction = nullptr;

	for (UUtilityAIAction* Action : InstancedActions)
	{
		Action->LastCanRun = !Action->IsMarkedForDeath() && Action->CanRun(Controller, Pawn);
		if (!Action->LastCanRun)
			continue;
		Action->LastScore = ScoreFilter(Action, Action->Score(Controller, Pawn));
		if (bIgnoreZeroScore && Action->LastScore == 0)
			continue;

		if (bUseLowestScore)
		{
			if (CheckLowestScore(Action, BestAction))
				BestAction = Action;
		}
		else
		{
			if (CheckHighestScore(Action, BestAction))
				BestAction = Action;
		}
	}

	return BestAction;
}

// Called every frame
void UUtilityAIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AAIController* Controller = Cast<AAIController>(GetOwner());
	if (!Controller)
		return;

	OnUtilityAIBeforeScoreComputation.Broadcast();

	APawn* Pawn = Controller->GetPawn();


	UUtilityAIAction* BestAction = nullptr;

	if (Pawn || bCanRunWithoutPawn)
	{
		BestAction = ComputeBestAction(Controller, Pawn);
	}


	if (BestAction)
	{
		OnUtilityAIActionChoosen.Broadcast(BestAction);
		if (LastAction != BestAction)
		{
			float CurrentTime = GetWorld()->GetTimeSeconds();
			// avoid too fast action switching
			if (LastSwitchTime == 0 || CurrentTime - LastSwitchTime > Bounciness)
			{
				OnUtilityAIActionChanged.Broadcast(BestAction, LastAction);
				if (LastAction)
				{
					LastAction->Exit(Controller, LastPawn);
				}
				BestAction->Enter(Controller, Pawn);
				LastSwitchTime = CurrentTime;
			}
			else
			{
				// fast exit if nothing to run
				if (!LastAction)
					return;
				BestAction = LastAction;
			}
		}
		BestAction->Tick(DeltaTime, Controller, Pawn);
		LastAction = BestAction;
		LastPawn = Pawn;
		OnUtilityAIActionTicked.Broadcast(BestAction);
		return;
	}

	OnUtilityAIActionNotAvailable.Broadcast();
	if (LastAction)
	{
		OnUtilityAIActionChanged.Broadcast(nullptr, LastAction);
		LastAction->Exit(Controller, LastPawn);
		LastAction->Resurrect();
		LastAction = nullptr;
		LastPawn = nullptr;
	}

}

TArray<UUtilityAIAction*> UUtilityAIComponent::GetActionInstances() const
{
	return InstancedActions.Array();
}

UUtilityAIAction* UUtilityAIComponent::GetActionInstanceByClass(TSubclassOf<UUtilityAIAction> ActionClass) const
{
	for (UUtilityAIAction* Action : InstancedActions)
	{
		if (Action->GetClass() == ActionClass)
			return Action;
	}
	return nullptr;
}

UUtilityAIAction* UUtilityAIComponent::GetCurrentActionInstance() const
{
	return LastAction;
}

TSubclassOf<UUtilityAIAction> UUtilityAIComponent::GetCurrentActionClass() const
{
	return LastAction ? LastAction->GetClass() : nullptr;
}

void UUtilityAIComponent::SetRandomStream(FRandomStream InRandomStream)
{
	RandomStream = InRandomStream;
	bUseRandomStream = true;
}

FRandomStream UUtilityAIComponent::GetRandomStream() const
{
	return RandomStream;
}

UUtilityAIAction* UUtilityAIComponent::ComputeBestAction(AAIController* Controller, APawn* Pawn)
{
	return ReceiveComputeBestAction(Controller, Pawn);
}
