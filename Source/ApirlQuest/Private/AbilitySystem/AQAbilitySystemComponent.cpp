#include "AbilitySystem/AQAbilitySystemComponent.h"

bool UAQAbilitySystemComponent::TryActivateAbilityByTag(const FGameplayTag& AbilityTag)
{
	FGameplayTagContainer TagContainer;

	TagContainer.AddTag(AbilityTag);
	return TryActivateAbilitiesByTag(TagContainer);
}

UGameplayAbility* UAQAbilitySystemComponent::FindAbilityInstanceByTag(const FGameplayTag& AbilityTag) const
{
    for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
    {
        if (Spec.Ability && Spec.Ability->AbilityTags.HasTag(AbilityTag))
        {
            // InstancedPerActor 模式下 GetPrimaryInstance() 返回唯一实例
            return Spec.GetPrimaryInstance();
        }
    }
    return nullptr;
}