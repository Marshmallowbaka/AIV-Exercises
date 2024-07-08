// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Ball.h"
#include "EnemyAIController.generated.h"

struct FAivState : public TSharedFromThis<FAivState>
{
private:
	TFunction<void(AAIController*)> Enter;
	TFunction<void(AAIController*)> Exit;
	TFunction<TSharedPtr<FAivState>(AAIController*, const float)> Tick;

public:
	FAivState()
	{
		Enter = nullptr;
		Exit = nullptr;
		Tick = nullptr;
	}

	FAivState(TFunction<void(AAIController*)> inEnter = nullptr,
	TFunction<void(AAIController*)> inExit = nullptr,
	TFunction<TSharedPtr<FAivState>(AAIController*, const float)> inTick = nullptr)
	{
		Enter = inEnter;
		Exit = inExit;
		Tick = inTick;
	}

	FAivState(const FAivState& Other) = delete;
	FAivState operator=(const FAivState& Other) = delete;
	FAivState(FAivState& Other) = delete;
	FAivState operator=(FAivState& Other) = delete;

	void CallEnter(AAIController* AIController)
	{
		if (Enter)
		{
			Enter(AIController);
		}
	}

	void CallExit(AAIController* AIController)
	{
		if (Exit)
		{
			Enter(AIController);
		}
	}

	TSharedPtr<FAivState> CallTick(AAIController* AIController, const float DeltaTime)
	{
		if (Tick)
		{
			TSharedPtr<FAivState> NewState = Tick(AIController, DeltaTime);

			if (NewState != nullptr && NewState != AsShared())
			{
				CallExit(AIController);
				NewState->CallEnter(AIController);
				return NewState;
			}
		}

		return AsShared();
	}
};

/**
 * 
 */
UCLASS()
class TAGGAME_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	TSharedPtr<FAivState> CurrentState;
	TSharedPtr<FAivState> GoToPlayer;
	TSharedPtr<FAivState> GoToBall;
	TSharedPtr<FAivState> GrabBall;
	TSharedPtr<FAivState> SearchForBall;

	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	ABall* BestBall;
};
