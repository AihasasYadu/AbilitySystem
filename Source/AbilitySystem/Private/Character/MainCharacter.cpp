// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MainCharacter.h"

#include "Player/GamePlayerState.h"


// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AMainCharacter::InitAbilitySystemComponent()
{
	AGamePlayerState* CurrentPlayerState = GetPlayerState<AGamePlayerState>();
	check(CurrentPlayerState);
	AbilitySystemComponent =  CastChecked<UGameAbilitySystemComponent>(CurrentPlayerState->GetAbilitySystemComponent());
	AbilitySystemComponent->InitAbilityActorInfo(CurrentPlayerState, this);
	AttributeSet = CurrentPlayerState->GetAttributeSet();
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMainCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilitySystemComponent();
	GiveDefaultAbilities();
	InitDefaultAttributes();
}

void AMainCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilitySystemComponent();
	InitDefaultAttributes();
}