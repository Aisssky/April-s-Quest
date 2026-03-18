#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AQGameplayAbility.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class APIRLQUEST_API UAQGameplayAbility : public UGameplayAbility 
{
	GENERATED_BODY()
public:
	UAQGameplayAbility();
	
	//Tag
	UPROPERTY(EditAnywhere,Category = "Ability")
	FGameplayTag ActivationTags;
	
protected:

	//便捷函数减少cast
	UFUNCTION(BlueprintCallable, Category = "Ability")
	AAQCharacterBase* GetAQCharacterFromActorInfo() const;

};
