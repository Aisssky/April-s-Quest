#include "UI/AQHUDPanel.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/AQCharacterBase.h"
#include "AbilitySystem/AQAttributeSet.h"

void UAQHUDPanel::NativeConstruct()
{
	Super::NativeConstruct();
	BindToPlayerCharacter();
}

void UAQHUDPanel::NativeDestruct()
{
	//先判空 是否已经被销毁
	if (UAQAttributeSet* AS = CachedAttributeSet.Get())
	{
		AS->OnHealthChanged.RemoveDynamic(this,&UAQHUDPanel::OnHealthChanged);
		AS->OnStaminaChanged.RemoveDynamic(this,&UAQHUDPanel::OnStaminaChanged);
	}

	//用委托句柄移除 解绑委托
	Super::NativeDestruct();
}

void UAQHUDPanel::BindToPlayerCharacter()
{
	AAQCharacterBase* Character = Cast<AAQCharacterBase>(UGameplayStatics::GetPlayerPawn(this, 0));

	if(!ensureMsgf(Character, TEXT("AQHUD:PlayerCharacter没找到!")))
	{
		return;
	}

	//AttributeSet挂在角色上 获取GetAttributeSet
	UAQAttributeSet* AS = Character->GetAttributeSet();
	if(!ensureMsgf(AS, TEXT("AQHUD:AttributeSet没找到!")))
	{
		return;
	}

	CachedAttributeSet = AS;//弱指针解绑用

	//绑定委托
	AS->OnHealthChanged.AddDynamic(this, &UAQHUDPanel::OnHealthChanged);
	AS->OnStaminaChanged.AddDynamic(this, &UAQHUDPanel::OnStaminaChanged);

	//绑定后立刻刷新UI，解决先有血量还是先有HUD的问题
	//防止 HUD 晚于 Character 创建导致初始值为 0
	OnHealthChanged(AS->GetHealth(), AS->GetMaxHealth());
	OnStaminaChanged(AS->GetStamina(), AS->GetMaxStamina());
}

void UAQHUDPanel::OnHealthChanged(float NewVaule, float MaxVaule)
{
	if (!PB_Health)return;

	//防除零崩溃
	const float Percent = MaxVaule > 0.f ? NewVaule / MaxVaule : 0.f;
	PB_Health->SetPercent(Percent);


	//更新数值文字
	if (TB_HealthText)
	{
		TB_HealthText->SetText(
			FText::FromString(FString::Printf(TEXT("%.0f/%.0f"), NewVaule, MaxVaule)));
	}
}

void UAQHUDPanel::OnStaminaChanged(float NewVaule, float MaxVaule)
{
	if (!PB_Stamina)return;

	//防除零崩溃
	const float Percent = MaxVaule > 0.f? NewVaule / MaxVaule : 0.f;
	PB_Stamina->SetPercent(Percent);


	//更新数值文字
	if (TB_StaminaText)
	{
		TB_StaminaText->SetText(
			FText::FromString(FString::Printf(TEXT("%.0f/%.0f"), NewVaule, MaxVaule)));
	}
}