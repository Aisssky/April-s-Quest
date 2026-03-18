
#include "Combat/AQCombatComponent.h"
#include "Characters/AQCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"


UAQCombatComponent::UAQCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}


void UAQCombatComponent::EnableAttack()
{
	bAttackEnabled = true;
	HitActors.Empty();
	SetComponentTickEnabled(true);
}

void UAQCombatComponent::DisableAttack()
{
	bAttackEnabled = false;
	SetComponentTickEnabled(false);
	HitActors.Empty();
}

void UAQCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bAttackEnabled)
	{
		PerformAttackTrace();
	}
}

void UAQCombatComponent::PerformAttackTrace()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter) return;

	USkeletalMeshComponent* Mesh = OwnerCharacter->GetMesh();
	if (!Mesh) return;

	FVector SocketLocation = Mesh->GetSocketLocation(AttackSocketName);

	//调试球型检测
	DrawDebugSphere(GetWorld(), SocketLocation, DetectionRadius, 12, FColor::Red, false, 0.1f);

	TArray<FOverlapResult> Overlaps;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(DetectionRadius);

	bool bHit = GetWorld()->OverlapMultiByChannel(
		Overlaps,
		SocketLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_Pawn, 
		Sphere
	);

	if (!bHit)return;

	for (FOverlapResult& Overlap : Overlaps)
	{
		AActor* HitActor = Overlap.GetActor();
		if (!HitActor)continue;

		//不打自己
		if (HitActor == GetOwner())continue;

		//防止动画打一次但是伤害n次
		if (HitActors.Contains(HitActor))continue;

		//打有ASC的角色
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor);
		if (!TargetASC)continue;

		//记录命中
		HitActors.Add(HitActor);

		//应用伤害
		if (!DamageEffect)continue;

		AAQCharacterBase* SourceCharacter = Cast<AAQCharacterBase>(GetOwner());	
		if (!SourceCharacter)continue;

		UAbilitySystemComponent* SourceASC = SourceCharacter->GetAbilitySystemComponent();	
		if (!SourceASC)continue;

		FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
		Context.AddSourceObject(SourceCharacter);

		FGameplayEffectSpecHandle Spec = SourceASC->MakeOutgoingSpec(DamageEffect, 1.f, Context);
		if (Spec.IsValid())
		{
			SourceASC->ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TargetASC);
		}
	}
	
}