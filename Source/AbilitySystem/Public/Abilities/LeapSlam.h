// LeapSlam.h
#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "LeapSlam.generated.h"

UCLASS()
class ABILITYSYSTEM_API ULeapSlam : public UGameplayAbility
{
	GENERATED_BODY()

public:
	ULeapSlam();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Leap Attack")
	float PreLeapDelay = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Leap Attack")
	float HitDelayAfterLaunch = 0.20f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Leap Attack")
	float RecoveryDelayAfterHit = 0.15f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Leap Attack")
	float LeapDistance = 700.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Leap Attack")
	float LeapZOffset = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Leap Attack")
	float LeapArc = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Leap Attack")
	float Damage = 25.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Leap Attack")
	float DamageTraceRadius = 70.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Leap Attack")
	float DamageTraceLength = 160.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Leap Attack")
	TEnumAsByte<ECollisionChannel> DamageTraceChannel = ECC_Pawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Leap Attack")
	bool bWaitForLandingBeforeEnding = true;

	UFUNCTION()
	void OnPreLeapDelayFinished();

	UFUNCTION()
	void OnLeapLaunched(FVector Destination);

	UFUNCTION()
	void OnLeapFailed(FVector Destination);

	UFUNCTION()
	void OnHitDelayFinished();

	UFUNCTION()
	void OnRecoveryDelayFinished();

	UFUNCTION()
	void OnCharacterLanded(const FHitResult& Hit);

	void StartLeap();
	void PerformDamageTrace();
	void FinishAbility(bool bWasCancelled);

private:
	UPROPERTY()
	TSet<TObjectPtr<AActor>> HitActorsThisActivation;

	bool bDamageWindowTriggered = false;
	bool bHasLanded = false;
};