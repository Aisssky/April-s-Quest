
#include "Abilities/AQGameplayAbility.h"
#include "Characters/AQCharacterBase.h"

UAQGameplayAbility::UAQGameplayAbility()
{
	//默认是instanced per execution
	//需要追踪技能状态，释放，使用，CD
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

}


AAQCharacterBase* UAQGameplayAbility::GetAQCharacterFromActorInfo() const
{
	//Actorinfo结构体
	//AvatarActor：执行技能的物理角色，可能为null	（Tab竟然连注释都能出吗，，强强
	return Cast<AAQCharacterBase>(GetAvatarActorFromActorInfo());
}

