// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "TagGameCharacter.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Ball.h"
#include "TagPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TAGGAME_API ATagPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ATagPlayerController();

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBlackboardData* BlackboardData;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UBlackboardComponent* BlackboardComponent;

	AActor* PlayerRef;


protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	bool CheckForBallCollision();
	/*void SetupInputComponent() override;
	void MoveForward(float value);
	void Turn(float value);*/
	void TraceForward();
	//void Jump();
};
