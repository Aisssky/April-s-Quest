#include "Characters/AQCharacterBase.h"
#include "AbilitySystem/AQAbilitySystemComponent.h"
#include "AbilitySystem/AQAttributeSet.h"
#include "Combat/AQCombatComponent.h"
#include "GameplayEffect.h"
#include "Abilities/GameplayAbility.h"
#include "Components/CapsuleComponent.h"    
#include "GameFramework/CharacterMovementComponent.h"


AAQCharacterBase::AAQCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UAQAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	
	AttributeSet = CreateDefaultSubobject<UAQAttributeSet>(TEXT("AttributeSet"));
	CombatComponent = CreateDefaultSubobject<UAQCombatComponent>(TEXT("CombatComponent"));
}

//GAS接口

UAbilitySystemComponent* AAQCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAQCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAQCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilitySystem();
}

//初始化

void AAQCharacterBase::InitAbilitySystem()
{

	if (bAbilitySystemInitialized)return;

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	ApplyDefaultEffects();
	GrantDefaultAbilities();

	bAbilitySystemInitialized = true;

}

void AAQCharacterBase::ApplyDefaultEffects()
{
	if (!AbilitySystemComponent || !DefaultAttributeEffect)return;

	FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
	Context.AddSourceObject(this);

	const FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1.f, Context);

	if (Spec.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}
}

void AAQCharacterBase::GrantDefaultAbilities()
{
	if (!AbilitySystemComponent)return;
	for (const TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities)
	{
		if (!AbilityClass)continue;
		
		FGameplayAbilitySpec Spec(AbilityClass, 1);

		AbilitySystemComponent->GiveAbility(Spec);
	}
}

//战斗接口
bool AAQCharacterBase::IsAlive() const
{
	return AttributeSet && AttributeSet->GetHealth() > 0;
}

float AAQCharacterBase::GetHealth() const
{
	return AttributeSet ? AttributeSet->GetHealth() : 0.f;
}

float AAQCharacterBase::GetMaxHealth() const
{
	return AttributeSet ? AttributeSet->GetMaxHealth() : 0.f;
}

void AAQCharacterBase::Die()
{
	if (!IsAlive())return;
	
	//取消所有技能的进行
	AbilitySystemComponent->CancelAllAbilities();

	//播放死亡动画
	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}

	//关闭碰撞然后去死
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();

	//TODO:死亡后的一些逻辑，比如掉落物品，销毁角色等
}