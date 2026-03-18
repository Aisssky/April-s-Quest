#pragma once

#include "CoreMinimal.h"
#include "Abilities/AQGameplayAbility.h"
#include "AQAbility_Attack.generated.h"


/**
 * 
 */
UCLASS()
class APIRLQUEST_API UAQAbility_Attack : public UAQGameplayAbility
{
	GENERATED_BODY()

public:
	UAQAbility_Attack();

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

	//蓝图编辑montage
	UPROPERTY(EditAnywhere, Category = "Ability")
	TObjectPtr<UAnimMontage> AttackMontage;

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

};

