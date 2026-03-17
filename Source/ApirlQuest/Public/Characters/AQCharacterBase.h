#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "AttributeSet.h"
#include "AQCharacterBase.generated.h"


class UAQAbilitySystemComponent;
class UAQAttributeSet;
class UAQCombatComponent;
class UGameplayEffect;
class UGameplayAbility;

/**
 * 
 */
UCLASS(Abstract)
class APIRLQUEST_API AAQCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AAQCharacterBase();

	//-IAbilitySystemInterface
	//GAS通过此接口找到ASC

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAQAttributeSet* GetAttributeSet() const { return AttributeSet; }

	//生命周期
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;//初始化

	//战斗接口
	UFUNCTION(BlueprintCallable, Category="Combat")
	virtual void Die();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	float GetMaxHealth() const;

protected:
	//GAS组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UAQAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UAQAttributeSet> AttributeSet;

	//战斗组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<UAQCombatComponent> CombatComponent;

	//初始化配置

	//初始属性
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS|Init")	
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;

	//初始技能
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Init")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	//死亡动画montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "Combat")
	TObjectPtr<UAnimMontage> DeathMontage;

	//内部初始化
	virtual void InitAbilitySystem();//注册属性和技能
	virtual void ApplyDefaultEffects();//应用初始属性
	virtual void GrantDefaultAbilities();//授予初始技能

private:
	bool bAbilitySystemInitialized = false;

};
