// Fill out your copyright notice in the Description page of Project Settings.


#include "TagPlayerController.h"
#include "TagGameGameMode.h"

ATagPlayerController::ATagPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BlackboardDataAsset(TEXT("/Game/Blueprints/BB_EnemyBoard"));
	
	if (BlackboardDataAsset.Succeeded())
	{
		BlackboardData = BlackboardDataAsset.Object;
	}

}

void ATagPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (BlackboardData)
	{
		BlackboardComponent->InitializeBlackboard(*BlackboardData);
	}

	BlackboardComponent->SetValueAsObject(FName("PlayerBall"), nullptr);
	BlackboardComponent->SetValueAsBool(FName("HaveToRun"), true);

	PlayerRef = GetWorld()->GetFirstPlayerController()->GetPawn();

}

void ATagPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckForBallCollision();
}

bool ATagPlayerController::CheckForBallCollision()
{
	if (BlackboardComponent->GetValueAsObject(FName("PlayerBall"))) return false;

	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	ATagGameGameMode* AIGameMode = Cast<ATagGameGameMode>(GameMode);
	const TArray<ABall*>& BallsList = AIGameMode->GetBalls();

	for (int32 i = 0; i < BallsList.Num(); i++)
	{
		if (!BallsList[i]->GetAttachParentActor())
		{
			float Dist = FVector::Distance(PlayerRef->GetActorLocation(),BallsList[i]->GetActorLocation());
			if (Dist < 3.0f && Dist > -3.0f)
			{
				BallsList[i]->SetActorLocation(FVector(0, 0, 0));
				BallsList[i]->AttachToActor(PlayerRef, FAttachmentTransformRules::KeepRelativeTransform);
				BlackboardComponent->SetValueAsObject(FName("PlayerBall"), BallsList[i]);
				BlackboardComponent->SetValueAsBool(FName("HaveToRun"), true);
				return true;
			}
		}
	}
	return false;
}

//void ATagPlayerController::SetupInputComponent()
//{
//	Super::SetupInputComponent();
//
//	 //Bind input axes
//	InputComponent->BindAxis("MoveForward", this, &ATagPlayerController::MoveForward);
//	InputComponent->BindAxis("Turn", this, &ATagPlayerController::Turn);
//
//	 //Bind input actions
//	//InputComponent->BindAction("Jump", IE_Pressed, this, &ATagPlayerController::Jump);
//}
//
//void ATagPlayerController::MoveForward(float Value)
//{
//	if (Value != 0.0f)
//	{
//		 //Move the character forward
//		APawn* ControlledPawn = GetPawn();
//		if (ControlledPawn)
//		{
//			ControlledPawn->AddMovementInput(ControlledPawn->GetActorForwardVector(), Value);
//		}
//	}
//}
//
//void ATagPlayerController::Turn(float Value)
//{
//	if (Value != 0.0f)
//	{
//		AddYawInput(Value);
//	}
//}
//
//void ATagPlayerController::Jump()
//{
//	APawn* ControlledPawn = GetPawn();
//	if (ControlledPawn)
//	{
//		* Character = Cast<ACharacter>(ControlledPawn);
//		if (Character)
//		{
//			Character->Jump();
//		}
//	}
//}

//void ATagPlayerController::TraceForward()
//{
//	FVector Start = GetPawn()->GetActorLocation();
//	FVector ForwardVector = GetPawn()->GetActorForwardVector();
//	FVector End = (ForwardVector * 1000.0f) + Start;  // Estendi la linea a 1000 unità davanti a te
//
//	FHitResult HitResult;
//
//	FCollisionQueryParams CollisionParams;
//	CollisionParams.AddIgnoredActor(this);  // Ignora il personaggio stesso
//
//	// Esegui il LineTrace (Raycast)
//	bool bIsHit = GetWorld()->LineTraceSingleByChannel(
//		HitResult,
//		Start,
//		End,
//		ECC_Visibility,
//		CollisionParams
//	);
//
//	// Visualizza la linea per il debug
//	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);
//
//	// Se qualcosa è stato colpito
//	if (bIsHit)
//	{
//		if (HitResult.Item != nullptr)
//		{
//			UE_LOG(LogTemp, Warning, TEXT("You are looking at: %s"), *HitResult.Actor->GetName());
//		}
//	}
//}