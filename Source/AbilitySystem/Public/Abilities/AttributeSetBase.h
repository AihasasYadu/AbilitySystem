#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AttributeSetBase.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class ABILITYSYSTEM_API UAttributeSetBase final : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UAttributeSetBase();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Ability | Gameplay Attribute")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, Health);

};