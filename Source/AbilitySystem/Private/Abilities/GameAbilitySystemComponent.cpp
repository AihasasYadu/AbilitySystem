#include "Abilities/GameAbilitySystemComponent.h"

void UGameAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

UGameAbilitySystemComponent::UGameAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
}
