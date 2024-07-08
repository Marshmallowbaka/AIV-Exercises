// Fill out your copyright notice in the Description page of Project Settings.


#include "BBAIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "TagGameGameMode.h"
ABBAIController::ABBAIController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BlackboardDataAsset(TEXT("/Game/Blueprints/BB_EnemyBoard"));
	if (BlackboardDataAsset.Succeeded())
	{
		BlackboardData = BlackboardDataAsset.Object;
	}

	CheckCollisionDistance = 80.f;
	TimerData = 0;
	TimerMax = .5f;
}

//substitute bestball wtih blackboard data

void ABBAIController::BeginPlay()
{
    Super::BeginPlay();
	
	if (BlackboardData)
	{
		BlackboardComponent->InitializeBlackboard(*BlackboardData);
	}

	GoToPlayer = MakeShared<FAivState>(
		[this](AAIController* AIController) {
			//UE_LOG(LogTemp, Display, TEXT("ENEMY : %s  ENTRATO IN STATO: GO TO PLAYER"), GetPawn()->GetFName());
			AIController->MoveToActor(AIController->GetWorld()->GetFirstPlayerController()->GetPawn(), CheckCollisionDistance);
		},
		nullptr,
		[this](AAIController* AIController, const float DeltaTime) -> TSharedPtr<FAivState> {
			
			if (GetScared())
			{
				//AIController->StopMovement();
				return RunAwayFromPlayer;
			}
			if (!GetBlackboardBall())
			{
				//AIController->StopMovement();
				return SearchForBall;
			}

			EPathFollowingStatus::Type State = AIController->GetMoveStatus();

			if (State == EPathFollowingStatus::Moving) return nullptr;

			ABall* BestBall = GetBlackboardBall();
			if (BestBall)
			{

				BestBall->SetActorRelativeLocation(FVector(0, 0, 0));
				BestBall->AttachToActor(nullptr, FAttachmentTransformRules::KeepRelativeTransform);
				BestBall->AttachToActor(GetWorld()->GetFirstPlayerController()->GetPawn(), FAttachmentTransformRules::KeepRelativeTransform);
				SetBlackboardBall(nullptr);
				SetPlayerBall(BestBall);
				SetScared(true);
				BestBall = nullptr;
			}
			
			return RunAwayFromPlayer;
		}
	);

	SearchForBall = MakeShared<FAivState>(
		[this](AAIController* AIController) {
			//UE_LOG(LogTemp, Display, TEXT("ENEMY : %s  ENTRATO IN STATO: SEARCH FOR BALL"), GetPawn()->GetFName());
			//if (GetBlackboardBall()) return;

			AGameModeBase* GameMode = AIController->GetWorld()->GetAuthGameMode();
			ATagGameGameMode* AIGameMode = Cast<ATagGameGameMode>(GameMode);
			const TArray<ABall*>& BallsList = AIGameMode->GetBalls();

			ABall* NearestBall = GetBlackboardBall();

			for (int32 i = 0; i < BallsList.Num(); i++)
			{
				if (/*BallsList[i] != GetBlackboardBall() &&*/
					BallsList[i] != GetPlayerBall() &&
					!BallsList[i]->GetAttachParentActor() && 
					(!NearestBall ||
						FVector::Distance(AIController->GetPawn()->GetActorLocation(), BallsList[i]->GetActorLocation()) <
						FVector::Distance(AIController->GetPawn()->GetActorLocation(), NearestBall->GetActorLocation()))
					)
				{
					NearestBall = BallsList[i];
				}
			}
			SetBlackboardBall(NearestBall);
		},
		nullptr,
		[this](AAIController* AIController, const float DeltaTime) -> TSharedPtr<FAivState> {
			if (GetBlackboardBall()) return GoToBall;
			if (GetScared()) return RunAwayFromPlayer;
			return nullptr;
		}
	);

	GoToBall = MakeShared<FAivState>(
		[this](AAIController* AIController) {
			//UE_LOG(LogTemp, Display, TEXT("ENEMY : %s  ENTRATO IN STATO: GO TO BALL"), GetPawn()->GetFName());
			if (GetBlackboardBall()->GetAttachParentActor()) 
				SetBlackboardBall(nullptr);
			if(GetBlackboardBall())
				AIController->MoveToActor(GetBlackboardBall(), CheckCollisionDistance);
		},
		nullptr,
		[this](AAIController* AIController, const float DeltaTime) -> TSharedPtr<FAivState> {
			if (!GetBlackboardBall()) return SearchForBall;
			EPathFollowingStatus::Type State = AIController->GetMoveStatus();
			if (State == EPathFollowingStatus::Moving)
			{
				if (GetScared()) return RunAwayFromPlayer;
				return nullptr;
			}
			//float Dist = FVector::Distance(GetBlackboardBall()->GetActorLocation(), GetPawn()->GetActorLocation());
			//float Dist = GetPawn()->GetActorLocation().Length() - GetBlackboardBall()->GetActorLocation().Length();
			//if(GetBlackboardBall() && ( Dist < CheckCollisionDistance))
			return GrabBall;
		}
	);

	GrabBall = MakeShared<FAivState>(
		[this](AAIController* AIController) {
			/*UE_LOG(LogTemp, Display, TEXT("ENEMY : %s  ENTRATO IN STATO: GRAB BALL"), GetPawn()->GetFName());
			if (GetBlackboardBall()->GetAttachParentActor())
			{
				SetBlackboardBall(nullptr);
			}*/
			if (GetBlackboardBall() == GetPlayerBall())
			{
				SetBlackboardBall(nullptr);
			}
		},
		nullptr,
		[this](AAIController* AIController, const float DeltaTime) -> TSharedPtr<FAivState> {
			if (!GetBlackboardBall())
			{
				//if (GetScared()) return RunAwayFromPlayer;
				return SearchForBall;
			}
			//if (BestBall->GetAttachParentActor() == AIController->GetPawn()) return GoToPlayer;
			GetBlackboardBall()->SetActorRelativeLocation(FVector(0, 0, 0));
			GetBlackboardBall()->AttachToActor(AIController->GetPawn(), FAttachmentTransformRules::KeepRelativeTransform);

			return GoToPlayer;
		}
	);

	RunAwayFromPlayer = MakeShared<FAivState>(
		[this](AAIController* AIController) {
			//UE_LOG(LogTemp, Display, TEXT("ENEMY : %s  ENTRATO IN STATO: RUN AWAY FROM PLAYER"), GetPawn()->GetFName());

			if (FVector::Distance(AIController->GetPawn()->GetActorLocation(), GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation()) <= 600.0f)
			{
				FVector Direction = AIController->GetPawn()->GetActorLocation() - AIController->GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
				Direction.Normalize();
				FVector MovingLocation = AIController->GetPawn()->GetActorLocation() + Direction * 600.0f;
				AIController->MoveToLocation(MovingLocation);
			}
			
		},
		nullptr,
		[this](AAIController* AIController, const float DeltaTime) -> TSharedPtr<FAivState> {
			//if (GetBlackboardBall() && GetScared()) return WaitingForLove;
			if (GetBlackboardBall() && !GetScared())
			{
				//AIController->StopMovement();
				return GoToPlayer;
			}

			if (!GetBlackboardBall() && !GetScared())
			{
				//AIController->StopMovement();
				return SearchForBall;
			}
			//else if (GetBlackboardBall() && GetScared()) return WaitingForLove;
			float Dist = FVector::Distance(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(), GetPawn()->GetActorLocation());
			if ( Dist < CheckCollisionDistance && !GetBlackboardBall())
			{
				/*AGameModeBase* GameMode = AIController->GetWorld()->GetAuthGameMode();
				ATagGameGameMode* AIGameMode = Cast<ATagGameGameMode>(GameMode);
				const TArray<ABall*>& BallsList = AIGameMode->GetBalls();
				for (int32 i = 0; i < BallsList.Num(); i++)
				{
					if (BallsList[i]->GetAttachParentActor() == GetWorld()->GetFirstPlayerController()->GetPawn())
					{
						
						SetBlackboardBall(BallsList[i]);

						BallsList[i]->SetActorRelativeLocation(FVector(0, 0, 0));
						BallsList[i]->AttachToActor(nullptr, FAttachmentTransformRules::KeepRelativeTransform);
						BallsList[i]->AttachToActor(this->GetPawn(), FAttachmentTransformRules::KeepRelativeTransform);
						SetScared(false);
						return SearchForBall;
					}
				}*/
				ABall* TempBall = GetPlayerBall();
				SetPlayerBall(nullptr);
				SetScared(false);
				TempBall->SetActorRelativeLocation(FVector(0, 0, 0));
				TempBall->AttachToActor(nullptr, FAttachmentTransformRules::KeepRelativeTransform);
				TempBall->AttachToActor(GetPawn(), FAttachmentTransformRules::KeepRelativeTransform);
				SetBlackboardBall(TempBall);
				return WaitingTo;
			}
			
			EPathFollowingStatus::Type State = AIController->GetMoveStatus();
			if (State == EPathFollowingStatus::Moving) return nullptr;
			return SearchForBall;
		}
	);

	WaitingTo = MakeShared<FAivState>(
		[this](AAIController* AIController) {
			//UE_LOG(LogTemp, Display, TEXT("ENEMY : %s  ENTRATO IN STATO: WAITING"), GetPawn()->GetFName());

			if (TimerData <= 0)
			{
				TimerData = TimerMax;
			}
		},
		nullptr,
		[this](AAIController* AIController, const float DeltaTime) -> TSharedPtr<FAivState> {
			TimerData -= DeltaTime;
			//UE_LOG(LogTemp, Display, TEXT("LO STATO DEL TIMER : %f"), TimerData);
			if (TimerData <= 0)
			{
				TimerData = 0;
				if (GetScared()) return RunAwayFromPlayer;
				return GoToPlayer;
			}
			return nullptr;
		}
	);

	CurrentState = SearchForBall;
	CurrentState->CallEnter(this);

}

void ABBAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentState)
	{
		CurrentState = CurrentState->CallTick(this, DeltaTime);
	}
	if (GetBlackboardBall() && GetBlackboardBall() == GetPlayerBall()) SetBlackboardBall(nullptr);
	CheckScare();
	//UE_LOG(LogTemp, Display, TEXT("GET SCARED VALUE %d"), GetScared());
}

ABall* ABBAIController::GetBlackboardBall() const
{
	if (BlackboardComponent)
	{
		return Cast<ABall>(BlackboardComponent->GetValueAsObject(FName("BestBall")));
	}
	return nullptr;
}

void ABBAIController::SetBlackboardBall(ABall* Ball)
{
	
	if (BlackboardComponent)
	{
		//if (Ball)
		//{
		//	Ball->AttachToActor(nullptr, FAttachmentTransformRules::KeepRelativeTransform);
		//	Ball->SetActorRelativeLocation(FVector(0, 0, 0));
		//	Ball->AttachToActor(GetPawn(), FAttachmentTransformRules::KeepRelativeTransform);
		//}
		//else if(!Ball && GetBlackboardBall())
		//{
		//	GetBlackboardBall()->AttachToActor(nullptr, FAttachmentTransformRules::KeepRelativeTransform);
		//}
		BlackboardComponent->SetValueAsObject(FName("BestBall"), Ball);
	}
	
}

bool ABBAIController::GetScared() const
{
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	ATagGameGameMode* AIGameMode = Cast<ATagGameGameMode>(GameMode);
	if (AIGameMode->BlackboardComponent)
	{
		return AIGameMode->BlackboardComponent->GetValueAsBool(FName("HaveToRun"));
	}
	return false;
}

void ABBAIController::SetScared(bool status)
{
	UE_LOG(LogTemp, Display, TEXT("SCARED STATUS = %d"), status);

	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	ATagGameGameMode* AIGameMode = Cast<ATagGameGameMode>(GameMode);
	if (AIGameMode->BlackboardComponent)
	{
		AIGameMode->BlackboardComponent->SetValueAsBool(FName("HaveToRun"), status);
	}
}

void ABBAIController::CheckScare()
{
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	ATagGameGameMode* AIGameMode = Cast<ATagGameGameMode>(GameMode);
	const TArray<ABall*>& BallsList = AIGameMode->GetBalls();
	for (int32 i = 0; i < BallsList.Num(); i++)
	{
		if (BallsList[i]->GetAttachParentActor() == GetPawn() && BallsList[i]->GetAttachParentActor() != GetBlackboardBall())
		{
			SetBlackboardBall(BallsList[i]);
		}

		if (BallsList[i]->GetAttachParentActor() == GetWorld()->GetFirstPlayerController()->GetPawn())
		{
			if (BallsList[i] == GetBlackboardBall()) SetBlackboardBall(nullptr);
			SetScared(true);
			return;
		}
	}
	SetScared(false);
}

void ABBAIController::SetPlayerBall(ABall* Ball)
{
	if (BlackboardComponent)
	{	
		/*if (Ball) 
		{
			Ball->SetActorRelativeLocation(FVector(0, 0, 0));
			Ball->AttachToActor(this->GetWorld()->GetFirstPlayerController()->GetPawn(), FAttachmentTransformRules::KeepRelativeTransform);
			SetScared(true);
		}
		else if(!Ball && GetPlayerBall())
		{
			GetPlayerBall()->AttachToActor(nullptr, FAttachmentTransformRules::KeepRelativeTransform);
			SetScared(false);
		}*/
		BlackboardComponent->SetValueAsObject(FName("PlayerBall"), Ball);
	}
	
}

ABall* ABBAIController::GetPlayerBall()
{
	if (BlackboardComponent)
	{
		return  Cast<ABall>(BlackboardComponent->GetValueAsObject(FName("PlayerBall")));
	}
	return nullptr;
}

