// LeapArc.cpp
#include "Abilities/LeapArc.h"

#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

ULeapArc* ULeapArc::LeapCharacterInArc(
	UGameplayAbility* OwningAbility,
	FName TaskInstanceName,
	float InLeapDistance,
	float InZOffset,
	float InArc,
	bool bInUseMovementInput,
	bool bInRotateToDirection)
{
	ULeapArc* Task = NewAbilityTask<ULeapArc>(OwningAbility, TaskInstanceName);
	Task->LeapDistance = InLeapDistance;
	Task->ZOffset = InZOffset;
	Task->Arc = InArc;
	Task->bUseMovementInput = bInUseMovementInput;
	Task->bRotateToDirection = bInRotateToDirection;
	return Task;
}

void ULeapArc::Activate()
{
	Super::Activate();

	ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
	if (!Character)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnFailed.Broadcast(FVector::ZeroVector);
		}
		EndTask();
		return;
	}

	FVector Direction = ResolveLeapDirection();
	if (Direction.IsNearlyZero())
	{
		Direction = Character->GetActorForwardVector();
	}

	Direction.Z = 0.f;
	Direction = Direction.GetSafeNormal();

	if (bRotateToDirection)
	{
		Character->SetActorRotation(Direction.Rotation());
	}

	const FVector Start = Character->GetActorLocation();
	FVector Destination = Start + Direction * LeapDistance;
	Destination.Z += ZOffset;

	FVector LaunchVelocity;
	const bool bHasSolution = UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this,
		LaunchVelocity,
		Start,
		Destination,
		0.f,
		Arc
	);

	if (!bHasSolution)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnFailed.Broadcast(Destination);
		}
		EndTask();
		return;
	}

	Character->LaunchCharacter(LaunchVelocity, true, true);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnLaunched.Broadcast(Destination);
	}

	EndTask();
}

FVector ULeapArc::ResolveLeapDirection() const
{
	ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
	if (!Character)
	{
		return FVector::ZeroVector;
	}

	if (!bUseMovementInput)
	{
		return Character->GetActorForwardVector();
	}

	AController* Controller = Character->GetController();
	if (!Controller)
	{
		return Character->GetActorForwardVector();
	}

	const FRotator ControlYawRot(0.f, Controller->GetControlRotation().Yaw, 0.f);
	const FVector Forward = FRotationMatrix(ControlYawRot).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(ControlYawRot).GetUnitAxis(EAxis::Y);

	const float MoveForward = Character->GetInputAxisValue(TEXT("MoveForward"));
	const float MoveRight = Character->GetInputAxisValue(TEXT("MoveRight"));

	FVector Direction = Forward * MoveForward + Right * MoveRight;
	if (Direction.IsNearlyZero())
	{
		Direction = Character->GetActorForwardVector();
	}

	return Direction.GetSafeNormal2D();
}