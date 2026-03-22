#include "AI/AQBTTask_UseAbility.h"
#include "AIController.h"
#include "AI/AQEnemyCharacterBase.h"
#include "AbilitySystem/AQAbilitySystemComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Abilities/GameplayAbility.h"

UAQBTTask_UseAbility::UAQBTTask_UseAbility()
{
	NodeName = TEXT("Use Ability By Tag");

	//每个AI独立一份Task对象 成员对象互不干扰
	bCreateNodeInstance = true; 

}

EBTNodeResult::Type UAQBTTask_UseAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!AIC)return EBTNodeResult::Failed;

	AAQEnemyCharacterBase* Enemy = Cast<AAQEnemyCharacterBase>(AIC->GetPawn());
	if (!Enemy)return EBTNodeResult::Failed;

	UAQAbilitySystemComponent* ASC = Cast<UAQAbilitySystemComponent>(Enemy->GetAbilitySystemComponent());
	if (!ASC)return EBTNodeResult::Failed;

	//激活
	bool bActivated = ASC ->TryActivateAbilityByTag(AbilityTag);
	if (!bActivated)return EBTNodeResult::Failed;

	//激活后拿到instance 绑定结束回调
	UGameplayAbility* AbilityInstance = ASC->FindAbilityInstanceByTag(AbilityTag);
	if (!AbilityInstance)
	{
		//技能激活后 因为条件不满足而结束的 视为成功
		return EBTNodeResult::Succeeded;
	}

	CachedBTComp = &OwnerComp;
	AbilityInstance->OnGameplayAbilityEnded.AddUObject(this, &UAQBTTask_UseAbility::OnAbilityInstanceEnded);

	//BT等待FinishLatentTask调用
	return EBTNodeResult::InProgress;

}

void UAQBTTask_UseAbility::OnAbilityInstanceEnded(UGameplayAbility* Ability)
{
	//解绑 防止重复调用
	if (Ability)
		Ability->OnGameplayAbilityEnded.RemoveAll(this);

	if (UBehaviorTreeComponent* BTComp = CachedBTComp.Get())
	{
		CachedBTComp.Reset();
		FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
	}
}

EBTNodeResult::Type UAQBTTask_UseAbility::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//BT被打断，取消技能 解绑回调
	if (AAIController* AIC = OwnerComp.GetAIOwner()) 
	{
		if (AAQCharacterBase* Enemy = Cast<AAQEnemyCharacterBase>(AIC->GetPawn()))
		{
			if (UAQAbilitySystemComponent* ASC = Cast<UAQAbilitySystemComponent>(Enemy->GetAbilitySystemComponent()))
			{
				//先解绑
				if (UGameplayAbility* Instance = ASC->FindAbilityInstanceByTag(AbilityTag))
				{
					Instance->OnGameplayAbilityEnded.RemoveAll(this);
				}
				//取消技能
				FGameplayTagContainer Tags(AbilityTag);
				ASC->CancelAbilities(&Tags);
			}
		}

	}
	CachedBTComp.Reset();
	return EBTNodeResult::Aborted;
}