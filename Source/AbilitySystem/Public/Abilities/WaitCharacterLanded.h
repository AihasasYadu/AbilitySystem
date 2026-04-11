// WaitCharacterLanded.h
#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "WaitCharacterLanded.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitCharacterLandedDelegate, const FHitResult&, Hit);

UCLASS()
class ABILITYSYSTEM_API UWaitCharacterLanded : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FWaitCharacterLandedDelegate OnLanded;

	UFUNCTION(BlueprintCallable, Category="Ability|Tasks",
		meta=(DisplayName="Wait Character Landed", HidePin="OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly="TRUE"))
	static UWaitCharacterLanded* WaitCharacterLanded(UGameplayAbility* OwningAbility, FName TaskInstanceName);

	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;

private:
	UFUNCTION()
	void HandleLanded(const FHitResult& Hit);

	TWeakObjectPtr<class ACharacter> CachedCharacter;
};