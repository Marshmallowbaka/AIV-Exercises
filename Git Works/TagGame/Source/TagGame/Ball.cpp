// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"

// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//generate mesh component and assing it to root
	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh")); 
	RootComponent = BallMesh;
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetAttachParentActor()) return;

	double Dist = FVector::Distance(GetActorLocation(), GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());
	if (Dist < 3.0f && Dist > -3.0f)
	{
		SetActorLocation(FVector(0, 0, 0));
		AttachToActor(GetWorld()->GetFirstPlayerController()->GetPawn(),FAttachmentTransformRules::KeepRelativeTransform);
	}
	
}

