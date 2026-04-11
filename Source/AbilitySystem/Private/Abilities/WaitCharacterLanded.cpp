// WaitCharacterLanded.cpp
#include "Abilities/WaitCharacterLanded.h"

#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"

UWaitCharacterLanded* UWaitCharacterLanded::WaitCharacterLanded(UGameplayAbility* OwningAbility, FName TaskInstanceName)
{
	return NewAbilityTask<UWaitCharacterLanded>(OwningAbility, TaskInstanceName);
}

void UWaitCharacterLanded::Activate()
{
	Super::Activate();

	ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
	if (!Character)
	{
		EndTask();
		return;
	}

	CachedCharacter = Character;
	Character->LandedDelegate.AddDynamic(this, &UWaitCharacterLanded::HandleLanded);
}

void UWaitCharacterLanded::HandleLanded(const FHitResult& Hit)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnLanded.Broadcast(Hit);
	}

	EndTask();
}

void UWaitCharacterLanded::OnDestroy(bool AbilityEnded)
{
	if (CachedCharacter.IsValid())
	{
		CachedCharacter->LandedDelegate.RemoveDynamic(this, &UWaitCharacterLanded::HandleLanded);
	}

	Super::OnDestroy(AbilityEnded);
}