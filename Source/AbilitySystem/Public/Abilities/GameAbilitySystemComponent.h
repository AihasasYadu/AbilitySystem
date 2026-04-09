// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ABILITYSYSTEM_API UGameAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
public:
	UGameAbilitySystemComponent();
};
