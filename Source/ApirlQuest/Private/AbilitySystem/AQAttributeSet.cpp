
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


	//元属性
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		float Damage = GetIncomingDamage();
		//消费后 清空IncomingDamage
		SetIncomingDamage(0.f);

		if (Damage > 0.f)
		{
			const float NewHealth = FMath::Max(GetHealth() - Damage,0.f);
			SetHealth(NewHealth);

			//广播死亡事件给CharacterBase
			OnHealthChanged.Broadcast(NewHealth, GetMaxHealth());


			//TODO: 死亡事件的委托，如果Health变为0了就触发死亡逻辑	
		}	
		return;
	}
	
	//Health
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		OnHealthChanged.Broadcast(GetHealth(), GetMaxHealth());
	}

	//Stamina
	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));
		OnStaminaChanged.Broadcast(GetStamina(), GetMaxStamina());
	}

	//MaxHealth
	else if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		OnHealthChanged.Broadcast(GetHealth(), GetMaxHealth());
	}

	//MaxStamina
	if (Data.EvaluatedData.Attribute == GetMaxStaminaAttribute())
	{
		OnStaminaChanged.Broadcast(GetStamina(), GetMaxStamina());
	}
}
