
#include "Abilities/AQAbility_Attack.h"
#include "GameFramework/Character.h"
#include "Characters/AQCharacterBase.h"


UAQAbility_Attack::UAQAbility_Attack()
{
}

void UAQAbility_Attack::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	//拿到角色
	AAQCharacterBase* Character = GetAQCharacterFromActorInfo();
	if (!Character || !AttackMontage)
	{
		//立刻结束
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	//绑定结束
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &UAQAbility_Attack::OnMontageEnded);
	}

	//播放动画
	Character->PlayAnimMontage(AttackMontage);
}

void UAQAbility_Attack::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	//因为先前只有一个montage所以跑的通 加一行montage过滤 
	if (Montage != AttackMontage)return;//只响应自己的Montage

	if(bInterrupted)
	{
		//如果被打断了，直接结束技能
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	//技能激活是异步的——ActivateAbility 调用完之后函数返回，但技能还在运行（Montage 还在播）。等到 Montage 结束触发回调时，早已不在 ActivateAbility 的调用栈里了，那三个参数早就没了。
	//GAS 在 ActivateAbility 开始时把这三个参数复制一份存到成员变量里，这样不管在哪个回调函数里，只要技能还在激活状态，Current 系列就一直有效。
	else
	{
		//动画正常结束，结束技能
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}

}

void UAQAbility_Attack::EndAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	bool bReplicateEndAbility, 
	bool bWasCancelled)
{
	
	//拿到角色
	AAQCharacterBase* Character = GetAQCharacterFromActorInfo();

	if (Character)
	{

		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		
		if(AnimInstance)
		{
			//解绑事件/防止内存泄漏
			AnimInstance->OnMontageEnded.RemoveDynamic(this, &UAQAbility_Attack::OnMontageEnded);
		}
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
