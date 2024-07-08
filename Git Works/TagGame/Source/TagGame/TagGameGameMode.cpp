// Copyright Epic Games, Inc. All Rights Reserved.

#include "TagGameGameMode.h"
#include "TagGameCharacter.h"
#include "TagPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/Actor.h"


ATagGameGameMode::ATagGameGameMode()
{
	// set default pawn class to our Blueprinted character
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BlackboardDataAsset(TEXT("/Game/Blueprints/BB_EnemyBoard"));
	if (BlackboardDataAsset.Succeeded())
	{
		BlackboardData = BlackboardDataAsset.Object;
	}

}

void ATagGameGameMode::BeginPlay()
{
	Super::BeginPlay();
	if (BlackboardData)
	{
		BlackboardComponent->InitializeBlackboard(*BlackboardData);
	}
	ResetMatch();

}

void ATagGameGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckForBallCollision();

	for (int32 i = 0; i < GameBalls.Num(); i++)
	{
		
		//if (GameBalls[i]->GetAttachParentActor() == Cast<AActor>(DefaultPawnClass)){}
		
		if (GameBalls[i]->GetAttachParentActor() != GetWorld()->GetFirstPlayerController()->GetPawn())
		{
			return;
		}
		SetScared(true);
		//ResetMatch();
	}
}

const TArray<ABall*>& ATagGameGameMode::GetBalls() const
{
	return GameBalls;
}

void ATagGameGameMode::ResetMatch()
{
	TargetPoints.Empty(); //svuoto l'array di target point
	GameBalls.Empty();

	for (TActorIterator<ATargetPoint> It(GetWorld()); It; ++It)
	{
		TargetPoints.Add(*It);
	}
	for (TActorIterator<ABall> It(GetWorld()); It; ++It)
	{
		if (It->GetAttachParentActor())
			It->AttachToActor(nullptr,FAttachmentTransformRules::KeepRelativeTransform);
		
		GameBalls.Add(*It);
	}

	TArray<ATargetPoint*> RandomTargetPoints = TargetPoints;

	for (int32 i = 0; i < GameBalls.Num(); i++)
	{
		const int32 index = FMath::RandRange(0, RandomTargetPoints.Num() - 1);
		GameBalls[i]->SetActorLocation(RandomTargetPoints[index]->GetActorLocation());
		RandomTargetPoints.RemoveAt(index);
	}
}

TArray<ATargetPoint*> ATagGameGameMode::GetTargetPoints() const
{
	return TargetPoints;
}


void ATagGameGameMode::SetScared(bool status)
{
	if (BlackboardComponent)
	{
		BlackboardComponent->SetValueAsBool(FName("HaveToRun"), status);
	}
}


bool ATagGameGameMode::CheckForBallCollision()
{
	if (BlackboardComponent->GetValueAsObject(FName("PlayerBall"))) return false;

	APawn* PlayerRef = GetWorld()->GetFirstPlayerController()->GetPawn();

	for (int32 i = 0; i < GameBalls.Num(); i++)
	{
		if (!GameBalls[i]->GetAttachParentActor())
		{
			float Dist = FVector::Distance(PlayerRef->GetActorLocation(), GameBalls[i]->GetActorLocation());
			if (Dist < 100.f)
			{
				GameBalls[i]->SetActorLocation(FVector(0, 0, 0));
				GameBalls[i]->AttachToActor(PlayerRef, FAttachmentTransformRules::KeepRelativeTransform);
				BlackboardComponent->SetValueAsObject(FName("PlayerBall"), GameBalls[i]);
				BlackboardComponent->SetValueAsBool(FName("HaveToRun"), true);
				return true;
			}
		}
	}
	return false;
}
