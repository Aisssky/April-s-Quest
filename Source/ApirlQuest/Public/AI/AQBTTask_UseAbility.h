#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GameplayTagContainer.h"
#include "AQBTTask_UseAbility.generated.h"

class UGameplayAbility;

UCLASS()
class APIRLQUEST_API UAQBTTask_UseAbility : public UBTTaskNode
{
	GENERATED_BODY()
public:

	UAQBTTask_UseAbility();
	
	//要激活的技能tag
	UPROPERTY(EditAnywhere,Category = "Ability")
	FGameplayTag AbilityTag;

protected:
	virtual EBTNodeResult::Type ExecuteTask(
		UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;
	virtual  EBTNodeResult::Type AbortTask(
		UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;

private:

	void OnAbilityInstanceEnded(UGameplayAbility* Ability);
	TWeakObjectPtr<UBehaviorTreeComponent> CachedBTComp;
};
