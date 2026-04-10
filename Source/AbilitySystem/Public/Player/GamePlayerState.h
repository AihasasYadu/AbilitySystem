// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "GamePlayerState.generated.h"

class UGameAbilitySystemComponent;
class UAttributeSetBase;

UCLASS()
class ABILITYSYSTEM_API AGamePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
protected:	
	UPROPERTY()
	TObjectPtr<UGameAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSetBase> AttributeSet;
	
public:
	AGamePlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UAttributeSetBase* GetAttributeSet() const;
};
