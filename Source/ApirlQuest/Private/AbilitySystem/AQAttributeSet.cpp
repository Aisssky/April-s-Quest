
#include "AbilitySystem/AQAttributeSet.h"
#include "GameplayEffectExtension.h"

UAQAttributeSet::UAQAttributeSet()
{
	//DataTable
}

void UAQAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}

}

void UAQAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		float Damage = GetIncomingDamage();
		//清空IncomingDamage
		SetIncomingDamage(0.f);

		if (Damage > 0.f)
		{
			const float NewHealth = FMath::Max(GetHealth() - Damage,0.f);
			SetHealth(NewHealth);

			//TODO:广播死亡事件给CharacterBase
		}
	}
}