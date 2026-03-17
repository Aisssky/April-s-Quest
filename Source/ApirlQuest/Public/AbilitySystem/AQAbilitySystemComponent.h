#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AQAbilitySystemComponent.generated.h"
/**
 * 
 */
UCLASS()
class APIRLQUEST_API UAQAbilitySystemComponent :public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	//通过TAG激活技能
	bool TryActivateAbilityByTag(const FGameplayTag& AbilityTag);
};
