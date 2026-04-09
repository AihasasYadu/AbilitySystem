#include "Abilities/AttributeSetBase.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UAttributeSetBase::UAttributeSetBase()
{
	InitHealth(80.f);
}

void UAttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}
