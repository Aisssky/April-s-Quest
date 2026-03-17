#include "AbilitySystem/AQAbilitySystemComponent.h"

bool UAQAbilitySystemComponent::TryActivateAbilityByTag(const FGameplayTag& AbilityTag)
{
	FGameplayTagContainer TagContainer;

	TagContainer.AddTag(AbilityTag);
	return TryActivateAbilitiesByTag(TagContainer);
}