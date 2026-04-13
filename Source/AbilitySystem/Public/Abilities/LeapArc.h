// LeapArc.h
#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "LeapArc.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLeapTaskResultDelegate, FVector, Destination);

UCLASS()
class ABILITYSYSTEM_API ULeapArc : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FLeapTaskResultDelegate OnLaunched;

	UPROPERTY(BlueprintAssignable)
	FLeapTaskResultDelegate OnFailed;

	UFUNCTION(BlueprintCallable, Category="Ability|Tasks",
		meta=(DisplayName="Leap Character In Arc", HidePin="OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly="TRUE"))
	static ULeapArc* LeapCharacterInArc(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName,
		float LeapDistance,
		float ZOffset,
		float Arc);

	virtual void Activate() override;

private:
	float LeapDistance = 700.f;
	float ZOffset = 100.f;
	float Arc = 0.5f;

	FVector ResolveLeapDirection() const;
};