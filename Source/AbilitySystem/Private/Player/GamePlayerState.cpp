#include "Player/GamePlayerState.h"

#include "AbilitySystemComponent.h"
#include "Abilities/AttributeSetBase.h"
#include "Abilities/GameAbilitySystemComponent.h"

AGamePlayerState::AGamePlayerState()
{
	SetNetUpdateFrequency(100.0f);
	
	AbilitySystemComponent = CreateDefaultSubobject<UGameAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet = CreateDefaultSubobject<UAttributeSetBase>("AttributeSet");
}

UAbilitySystemComponent* AGamePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSetBase* AGamePlayerState::GetAttributeSet() const
{
	return AttributeSet;
}
