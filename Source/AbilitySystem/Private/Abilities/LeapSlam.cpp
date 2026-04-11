// LeapSlam.cpp
#include "Abilities/LeapSlam.h"

#include "Abilities/LeapArc.h"
#include "Abilities/WaitCharacterLanded.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

ULeapSlam::ULeapSlam()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void ULeapSlam::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	HitActorsThisActivation.Reset();
	bDamageWindowTriggered = false;
	bHasLanded = false;

	if (bWaitForLandingBeforeEnding)
	{
		UWaitCharacterLanded* LandedTask = UWaitCharacterLanded::WaitCharacterLanded(this, TEXT("WaitForLanded"));
		LandedTask->OnLanded.AddDynamic(this, &ULeapSlam::OnCharacterLanded);
		LandedTask->ReadyForActivation();
	}

	if (PreLeapDelay <= 0.f)
	{
		StartLeap();
		return;
	}

	UAbilityTask_WaitDelay* DelayTask = UAbilityTask_WaitDelay::WaitDelay(this, PreLeapDelay);
	DelayTask->OnFinish.AddDynamic(this, &ULeapSlam::OnPreLeapDelayFinished);
	DelayTask->ReadyForActivation();
}

void ULeapSlam::OnPreLeapDelayFinished()
{
	StartLeap();
}

void ULeapSlam::StartLeap()
{
	ULeapArc* LeapTask = ULeapArc::LeapCharacterInArc(
		this,
		TEXT("LeapCharacter"),
		LeapDistance,
		LeapZOffset,
		LeapArc,
		bUseMovementInput,
		bRotateToDirection
	);

	LeapTask->OnLaunched.AddDynamic(this, &ULeapSlam::OnLeapLaunched);
	LeapTask->OnFailed.AddDynamic(this, &ULeapSlam::OnLeapFailed);
	LeapTask->ReadyForActivation();
}

void ULeapSlam::OnLeapLaunched(FVector Destination)
{
	UAbilityTask_WaitDelay* HitDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, HitDelayAfterLaunch);
	HitDelayTask->OnFinish.AddDynamic(this, &ULeapSlam::OnHitDelayFinished);
	HitDelayTask->ReadyForActivation();
}

void ULeapSlam::OnLeapFailed(FVector Destination)
{
	FinishAbility(true);
}

void ULeapSlam::OnHitDelayFinished()
{
	bDamageWindowTriggered = true;
	PerformDamageTrace();

	if (!bWaitForLandingBeforeEnding)
	{
		UAbilityTask_WaitDelay* RecoveryTask = UAbilityTask_WaitDelay::WaitDelay(this, RecoveryDelayAfterHit);
		RecoveryTask->OnFinish.AddDynamic(this, &ULeapSlam::OnRecoveryDelayFinished);
		RecoveryTask->ReadyForActivation();
		return;
	}

	if (bHasLanded)
	{
		UAbilityTask_WaitDelay* RecoveryTask = UAbilityTask_WaitDelay::WaitDelay(this, RecoveryDelayAfterHit);
		RecoveryTask->OnFinish.AddDynamic(this, &ULeapSlam::OnRecoveryDelayFinished);
		RecoveryTask->ReadyForActivation();
	}
}

void ULeapSlam::OnCharacterLanded(const FHitResult& Hit)
{
	bHasLanded = true;

	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (Character && Character->GetCharacterMovement())
	{
		Character->GetCharacterMovement()->StopMovementImmediately();
	}

	if (bDamageWindowTriggered)
	{
		UAbilityTask_WaitDelay* RecoveryTask = UAbilityTask_WaitDelay::WaitDelay(this, RecoveryDelayAfterHit);
		RecoveryTask->OnFinish.AddDynamic(this, &ULeapSlam::OnRecoveryDelayFinished);
		RecoveryTask->ReadyForActivation();
	}
}

void ULeapSlam::OnRecoveryDelayFinished()
{
	FinishAbility(false);
}

void ULeapSlam::PerformDamageTrace()
{
	ACharacter* Character = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!Character)
	{
		return;
	}

	const FVector Start = Character->GetActorLocation() + Character->GetActorForwardVector() * 60.f + FVector(0.f, 0.f, 50.f);
	const FVector End = Start + Character->GetActorForwardVector() * DamageTraceLength;

	TArray<FHitResult> Hits;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Character);

	const bool bHit = UKismetSystemLibrary::SphereTraceMulti(
		Character,
		Start,
		End,
		DamageTraceRadius,
		UEngineTypes::ConvertToTraceType(DamageTraceChannel),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		Hits,
		true
	);

	if (!bHit)
	{
		return;
	}

	for (const FHitResult& Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor || HitActorsThisActivation.Contains(HitActor))
		{
			continue;
		}

		HitActorsThisActivation.Add(HitActor);
		UGameplayStatics::ApplyDamage(HitActor, Damage, Character->GetController(), Character, nullptr);
	}
}

void ULeapSlam::FinishAbility(bool bWasCancelled)
{
	if (!IsActive())
	{
		return;
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, bWasCancelled);
}

void ULeapSlam::EndAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility,
	bool bWasCancelled)
{
	HitActorsThisActivation.Reset();
	bDamageWindowTriggered = false;
	bHasLanded = false;

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}