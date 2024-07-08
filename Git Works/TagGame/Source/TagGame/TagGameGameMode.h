// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/TargetPoint.h"
#include "Ball.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "TagGameGameMode.generated.h"

UCLASS(minimalapi)
class ATagGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	TArray<ATargetPoint*> TargetPoints;
	TArray<ABall*> GameBalls;
	void ResetMatch();

public:
	ATagGameGameMode();
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	const TArray<ABall*>& GetBalls() const;
	TArray<ATargetPoint*> GetTargetPoints() const;
	void SetScared(bool status);
	bool CheckForBallCollision();

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBlackboardData* BlackboardData;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UBlackboardComponent* BlackboardComponent;

	void SpawnBalls();
};



