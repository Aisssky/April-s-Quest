#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AQCombatComponent.generated.h"
/**
 * 
 */
class UGameplayEffect;


UCLASS(ClassGroup=(Custom),meta=(BlueprintSpawnableComponent))
class APIRLQUEST_API UAQCombatComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UAQCombatComponent();

	//给ANS蓝图的接口
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EnableAttack();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void DisableAttack();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetAttackSocketName(FName SocketName) { AttackSocketName=SocketName; }

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void SetDetectionRadius(float Radius) { DetectionRadius = Radius; }

	//BP_PlayerCharacter里面配置
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<UGameplayEffect> DamageEffect;

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void PerformAttackTrace();

	bool bAttackEnabled = false;
	FName AttackSocketName = TEXT("hand_rSword");
	float DetectionRadius = 50.f;

	//防止动画打一次但是伤害n次
	TArray<TWeakObjectPtr<AActor>> HitActors;
};
